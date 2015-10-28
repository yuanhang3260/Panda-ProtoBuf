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
  // Sets rpc internal flag rpc_finished_ = false so that next call of
  // rpc->Wait() may properly block.
  rpc->SetRpcStart();

  // Init Channel.
  rpc_client_channel_->Initialize();
  // Check channel is connected.
  if (!rpc_client_channel_->connected() &&
      rpc_client_channel_->ConnectToServer() < 0) {
    rpc->set_client_status(Rpc::INTERNAL_CHANNEL_ERROR);
    rpc->SetRpcFinished();
    return;
  }

  // Synchronous style.
  // DoRpcCall(rpc, descriptor, method_name, request, response, cb);

  // Asynchronously style.
  std::thread t(&RpcService::DoRpcCall, this,
                rpc, descriptor, method_name, request, response, cb);
  t.detach();
}

void RpcService::DoRpcCall(Rpc* rpc,
                           const RpcDescriptor* descriptor,
                           std::string method_name,
                           const proto::Message* request,
                           proto::Message* response,
                           Base::Closure* cb) {
  if (ClientSendRequest(rpc, descriptor, method_name, request) < 0) {
    FinishRpcCall(rpc);
    return;
  }

  if (ClientReceiveResponse(rpc, response, cb) < 0) {
    FinishRpcCall(rpc);
    return;
  }

  rpc->set_client_status(Rpc::SUCCESS);

  // Run the callback
  if (cb) {
    cb->Run();
    delete cb;
  }

  FinishRpcCall(rpc);
}

void RpcService::FinishRpcCall(Rpc* rpc) {
  if (!options_.keep_alive) {
    rpc_client_channel_->Disconnect();
  }
  // Set rpc finished so that future rpc->Wait() will not block.
  rpc->SetRpcFinished();
}

int RpcService::ClientSendRequest(Rpc* rpc,
                                  const RpcDescriptor* descriptor,
                                  std::string method_name,
                                  const proto::Message* request) {
  // Serialize the request message
  proto::SerializedMessage* sdreq = request->Serialize();
  const char* req_data = sdreq->GetBytes();

  // Create a request header and set it properly.
  RpcRequestHeader request_header;
  request_header.set_service_name(descriptor->fullname());
  request_header.set_method_name(method_name);
  request_header.set_rpc_request_length(sdreq->size());
  request_header.set_keep_alive(options_.keep_alive);

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

  rpc_client_channel_->SendData(reinterpret_cast<const char*>(&req_header_size),
                                sizeof(req_header_size));

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

  // Check rpc response status in response header
  rpc->SetRpcReturnCode(response_header.rpc_return_code());
  rpc->SetRpcReturnMesssage(response_header.rpc_return_msg());
  if (response_header.rpc_return_code() != RpcResponseHeader::OK) {
    rpc->set_client_status(Rpc::BAD_RESPONSE);
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
