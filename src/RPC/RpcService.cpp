#include <time.h>
#include <chrono>
#include <thread>

#include "Utility/Utils.h"
#include "Proto/SerializedMessage.h"
#include "RpcService.h"
#include "RpcSession_pb.h"

namespace RPC {

RpcService::RpcService(const std::string name) : name_(name) {
}

RpcService::~RpcService() {
}

// ----------------------------- Server methods ----------------------------- //
void RpcService::UnInplemented(Rpc* rpc, Base::Closure* done) {
  (void)rpc;
  if (done) {
    done->Run();
  }
}

// ----------------------------- Client methods ----------------------------- //
void RpcService::InitStub(std::string name,
                          ::RPC::RpcClientChannel* channel,
                          const ::RPC::RpcStubOptions options) {
  name_ = name;
  rpc_client_channel_.reset(channel);
  options_ = options;
}

void RpcService::StartClientRpcCall(
    Rpc* rpc,
    const RpcDescriptor* descriptor,
    std::string method_name,
    const proto::Message* request,
    proto::Message* response,
    Base::Closure* cb) {
  // Init Channel
  rpc_client_channel_->Initialize();

  // Synchronous style ?
  // DoRpcCall(rpc, descriptor, method_name, request, response, cb);

  // Do rpc call asynchronously.
  std::thread t(&RpcService::DoRpcCall, this,
                rpc, descriptor, method_name, request, response, cb);
  t.join();
}

void RpcService::DoRpcCall(Rpc* rpc,
                           const RpcDescriptor* descriptor,
                           std::string method_name,
                           const proto::Message* request,
                           proto::Message* response,
                           Base::Closure* cb) {
  if (ClientSendRequest(rpc, descriptor, method_name, request) < 0) {
    rpc->SetRpcFinished();
    return;
  }

  if (ClientReceiveResponse(rpc, response, cb) < 0) {
    rpc->SetRpcFinished();
    return;
  }

  // Run the callback
  if (cb) {
    cb->Run();
    delete cb;
  }

  // Set rpc finished so that future rpc->Wait() will not block.
  rpc->set_client_status(Rpc::SUCCESS);
  rpc->SetRpcFinished();
}

int RpcService::ClientSendRequest(Rpc* rpc,
                                  const RpcDescriptor* descriptor,
                                  std::string method_name,
                                  const proto::Message* request) {
  // Init a request header.
  RpcRequestHeader request_header;
  request_header.set_service_name(descriptor->fullname());
  request_header.set_method_name(method_name);

  // Serialize the request message
  proto::SerializedMessage* sdreq = request->Serialize();
  const char* req_data = sdreq->GetBytes();

  // Set request size in request header
  request_header.set_rpc_request_length(sdreq->size());

  // Serialize the request header
  proto::SerializedMessage* sdhdr = request_header.Serialize();
  const char* hdr_data = sdhdr->GetBytes();

  // Begin sending data
  if (!rpc_client_channel_ || !rpc_client_channel_->IsReady()) {
    rpc->set_client_status(Rpc::INTERNAL_CHANNEL_ERROR);
    return -1;
  }

  // Send packet header:
  // | check number | header-length | request-length | ... data ... |
  //      4 byte          4 byte          4 byte
  int check_num = Utils::RandomNumber();
  rpc->set_check_num(check_num);  // set check_num
  int req_header_size = sdhdr->size(); // request header size
  int req_size = sdreq->size();  // user request size

  rpc_client_channel_->SendData(reinterpret_cast<const char*>(&check_num),
                                sizeof(check_num));
  rpc_client_channel_->FlushSend();
  std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
  
  rpc_client_channel_->SendData(reinterpret_cast<const char*>(&req_header_size),
                                sizeof(req_header_size));
  rpc_client_channel_->FlushSend();
  std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
  
  rpc_client_channel_->SendData(reinterpret_cast<const char*>(&req_size),
                                sizeof(req_size));
  rpc_client_channel_->FlushSend();
  std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));

  // Send request header and request message.
  rpc_client_channel_->SendData(hdr_data, sdhdr->size());
  rpc_client_channel_->FlushSend();
  std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));

  rpc_client_channel_->SendData(req_data, sdreq->size());

  // Flush send channel to make sure data is sent.
  rpc_client_channel_->FlushSend();

  delete sdreq;
  delete sdhdr;
  return 0;
}

int RpcService::ClientReceiveResponse(Rpc* rpc,
                                      proto::Message* response,
                                      Base::Closure* cb) {
  // Response packet:
  // | check number | header-length | response-length | ... data ... |
  //      4 byte          4 byte          4 byte

  // Receive check_num
  int check_num = 0;
  int nread = rpc_client_channel_->ReceiveData(
      reinterpret_cast<char*>(&check_num), sizeof(int));
  if (nread != sizeof(int)) {
    rpc->set_client_status(Rpc::INTERNAL_CHANNEL_ERROR);
    return -1;
  }
  // check check_num
  if (check_num != rpc->check_num()) {
    rpc->set_client_status(Rpc::CHECK_NUM_ERROR);
    return -1;
  }

  // Get response header length
  int header_size = 0;
  nread = rpc_client_channel_->ReceiveData(
      reinterpret_cast<char*>(&header_size), sizeof(int));
  if (nread != sizeof(int)) {
    rpc->set_client_status(Rpc::INTERNAL_CHANNEL_ERROR);
    return -1;
  }
  if (header_size < 0) {
    rpc->set_client_status(Rpc::RESPONSE_HEADER_SIZE_ERROR);
    return -1;
  }

  // Get response length
  int res_size = 0;
  nread = rpc_client_channel_->ReceiveData(
      reinterpret_cast<char*>(&res_size), sizeof(int));
  if (nread != sizeof(int)) {
    rpc->set_client_status(Rpc::INTERNAL_CHANNEL_ERROR);
    return -1;
  }
  if (res_size < 0) {
    rpc->set_client_status(Rpc::RESPONSE_SIZE_ERROR);
    return -1;
  }

  // Receive response header
  char hdr_buf[header_size];
  nread = rpc_client_channel_->ReceiveData(hdr_buf, header_size);
  if (nread != header_size) {
    rpc->set_client_status(Rpc::INTERNAL_CHANNEL_ERROR);
    return -1;
  }
  // Deserialize response header.
  RpcResponseHeader response_header;
  response_header.DeSerialize(hdr_buf, header_size);
  if (!response_header.has_rpc_response_length() ||
      response_header.rpc_response_length() != (unsigned)res_size) {
    rpc->set_client_status(Rpc::BAD_RESPONSE_HEADER);
    return -1;
  }

  // Receive response
  char res_buf[res_size];
  nread = rpc_client_channel_->ReceiveData(res_buf, res_size);
  if (nread != res_size) {
    rpc->set_client_status(Rpc::INTERNAL_CHANNEL_ERROR);
    return -1;
  }
  // Deserialize response
  response->DeSerialize(res_buf, res_size);
  return 0;
}

}  // namespace RPC
