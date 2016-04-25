#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <assert.h>
#include <iostream>

#include "RpcServer.h"

const char* lineseperator = "*************************************************";

namespace RPC {

RpcServer::RpcServer(int port) : recv_em_(7), run_em_(5), port_(port) {
}

RpcServer::RpcServer(int port, int recv_em_size) :
    recv_em_(recv_em_size),
    port_(port) {
}

void RpcServer::StartServing() {
  // Create listening socket.
  listen_socket_.reset(Network::Socket::CreateServerSocket(port_, true));
  if (listen_socket_->closed()) {
    fprintf(stderr, "FAIL: Server not successfully initialized yet.\n");
    return;
  }

  std::cout << lineseperator << std::endl;
  std::cout << "Start serving" << std::endl;
  recv_em_.AddTask(
      Base::NewCallBack(&RpcServer::RpcConnectionListenerHandler, this));;
  recv_em_.Start();
  run_em_.Start();

  recv_em_.AwaitTermination();
  run_em_.AwaitTermination();
}

void RpcServer::RpcConnectionListenerHandler() {
  struct sockaddr_in cli_addr;
  unsigned int clilen = sizeof(cli_addr);
  /* Accept actual connection from the client */
  while (1) {
    int newsockfd = accept(listen_socket_->getFd(),
                           (struct sockaddr*)&cli_addr,
                           &clilen);
    int x = fcntl(newsockfd, F_GETFL, 0);
    fcntl(newsockfd, F_SETFL, x | O_NONBLOCK);

    static int num = 0;
    printf("*************** Accept %d *******************\n", ++num);
    if (newsockfd < 0) {
      std::cerr << "ERORR: Accept connection failed" << std::endl;
      continue;
    }

    // Add rpc session
    {
      std::unique_lock<std::mutex> lock(map_mutex_);
      if (sessions_map_.find(newsockfd) == sessions_map_.end()) {
        sessions_map_[newsockfd] = new RpcSession(newsockfd);
      }
    }
    // Submit it to event manager.
    recv_em_.AddTaskWaitingReadable(newsockfd,
        Base::NewCallBack(&RpcServer::ReadRpcRequestHandler, this, newsockfd));
  }
}

void RpcServer::ReadRpcRequestHandler(int fd) {
  std::cout << "Read_Handler(" << fd << ") ..." << std::endl;
  
  // Find the session control block.
  RpcSession* session = nullptr;
  {
    std::unique_lock<std::mutex> lock(map_mutex_);
    if (sessions_map_.find(fd) == sessions_map_.end()) {
      sessions_map_[fd] = new RpcSession(fd);
    }
    session = sessions_map_.at(fd);
  }
  RpcServerChannel* channel = session->channel();
  char* buf = session->InternalBuf();

  // Read rpc packet header;
  int nread = -1;
  if (session->state() == RpcSession::INIT ||
      session->state() == RpcSession::PARSING_PKT_HDR) {
    while (!session->bufFull() &&
           (nread = channel->ReceiveData(buf + session->received_size(),
                                         session->remain_size_to_recv())) > 0) {
      printf("#1 nread = %d\n", nread);
      session->add_received_size(nread);
      if (session->bufFull()) {
        // Done reading packet header. Parse the header and reset internal
        // buffer. And change state to PARSING_REQ_HDR.
        if (ParseRpcPacketHeader(session) < 0) {
          session->set_state(RpcSession::REQUEST_ERROR);
          EnqueueRpcBanckendProcessing(session);
          return;
        }
        session->ResetBuf(session->req_header_size());
        session->set_state(RpcSession::PARSING_REQ_HDR);
        break;
      }
    }
    if ((nread == 0 && !session->bufFull()) ||
        (errno != EAGAIN && errno != 0 && errno != ECONNRESET)) {
      RemoveSession(fd);
      return;
    }
  }

  // Read request header
  buf = session->InternalBuf();
  if (session->state() == RpcSession::PARSING_REQ_HDR) {
    while (!session->bufFull() &&
           (nread = channel->ReceiveData(buf + session->received_size(),
                                         session->remain_size_to_recv())) > 0) {
      printf("#2 nread = %d\n", nread);
      session->add_received_size(nread);
      if (session->bufFull()) {
        // Done reading rpc request header. Deserialize RpcRequestHeader and
        // do sanity check.
        if (ParseRpcRequestHeader(session) < 0) {
          session->set_state(RpcSession::REQUEST_ERROR);
          EnqueueRpcBanckendProcessing(session);
          return;
        }
        session->ResetBuf(session->req_size());
        session->set_state(RpcSession::PARSING_REQ);
        break;
      }
    }
    // Unexpected EOF or errno other than EAGAIN.
    if ((nread == 0 && !session->bufFull()) ||
        (errno != EAGAIN && errno != 0 && errno != ECONNRESET)) {
      RemoveSession(fd);
      return;
    }
  }

  // Read rpc request
  buf = session->InternalBuf();
  if (session->state() == RpcSession::PARSING_REQ) {
    while (!session->bufFull() &&
           (nread = channel->ReceiveData(buf + session->received_size(),
                                         session->remain_size_to_recv())) > 0) {
      printf("#3 nread = %d\n", nread);
      session->add_received_size(nread);
      if (session->bufFull()) {
        // Done reading rpc request. Deserialize request and process.
        if (ParseRpcRequest(session) < 0) {
          session->set_state(RpcSession::REQUEST_ERROR);
          EnqueueRpcBanckendProcessing(session);
          return;
        }
        session->ResetBuf(0);
        session->set_state(RpcSession::READ_DONE);
        break;
      }
    }
    // Unexpected EOF or errno other than EAGAIN.
    if ((nread == 0 && !session->bufFull()) ||
        (errno != EAGAIN && errno != 0 && errno != ECONNRESET)) {
      RemoveSession(fd);
      return;
    }
  }

  // If read done, excute the rpc method in background thread pool.
  if (session->state() == RpcSession::READ_DONE) {
    EnqueueRpcBanckendProcessing(session);
  }
  else {
    // Continue reading request.
    recv_em_.ModifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
        Base::NewCallBack(&RpcServer::ReadRpcRequestHandler, this, fd));
  }
}

void RpcServer::WriteRpcResponseHandler(int fd) {
  std::cout << "Write_Handler(" << fd << ") ..." << std::endl;

  // Find the session control block.
  RpcSession* session = nullptr;
  {
    // TODO: handle no seesion error
    std::unique_lock<std::mutex> lock(map_mutex_);
    if (sessions_map_.find(fd) == sessions_map_.end()) {
      RemoveSession(fd);
      return;
    }
    session = sessions_map_.at(fd);
  }
  RpcServerChannel* channel = session->channel();

  // Rpc method execution done - prepare for data to send
  if (session->state() == RpcSession::RPC_METHOD_DONE) {
    PrepareResponseData(session);
    session->set_state(RpcSession::WRITING_SUCCESS_RES);
  }

  if (session->state() == RpcSession::REQUEST_ERROR) {
    PrepareResponseData(session);
    session->set_state(RpcSession::WRITING_ERROR_RES);
  }

  if (session->state() == RpcSession::WRITING_SUCCESS_RES ||
      session->state() == RpcSession::WRITING_ERROR_RES) {
    char* buf = session->InternalBuf();
    int nwrite = channel->socket()->Write(buf + session->sent_size(),
                                          session->remain_size_to_send());
    std::cout << "nwrite = " << nwrite << std::endl;
    if (nwrite < 0) {
      RemoveSession(fd);
      return;
    }
    session->add_sent_size(nwrite);
    if (session->sent_size() == session->bufSize()) {
      // Only keep-alive if last rpc call is succss.
      if (session->keep_alive() &&
          session->state() == RpcSession::WRITING_SUCCESS_RES) {
        session->set_state(RpcSession::INIT);
        session->ResetAll();
        recv_em_.AddTaskWaitingReadable(fd,
          Base::NewCallBack(&RpcServer::ReadRpcRequestHandler, this, fd));
      }
      else {
        RemoveSession(fd);
      }
    }
    else {
      // Continue writing response.
      run_em_.ModifyTaskWaitingStatus(fd, EPOLLOUT | EPOLLONESHOT,
          Base::NewCallBack(&RpcServer::WriteRpcResponseHandler, this, fd));
    }
  }
  return;
}

void RpcServer::EnqueueRpcBanckendProcessing(RpcSession* session) {
  recv_em_.RemoveAwaitingTask(session->getFd());
  run_em_.AddTask(Base::NewCallBack(
                      &RpcServer::BackendRpcProcess, this, session));
}

void RpcServer::BackendRpcProcess(RpcSession* session) {
  // Run rpc task.
  if (session->state() == RpcSession::READ_DONE &&
      session->handler()->rpc_method) {
    (session->handler()->rpc_method)(session->rpc());
    session->rpc()->SetRpcReturnCode(RpcResponseHeader::OK);
    session->set_state(RpcSession::RPC_METHOD_DONE);
  }

  // Send reply.
  WriteRpcResponseHandler(session->getFd());
}

void RpcServer::RemoveSession(int fd) {
  std::cout << "Removing session " << fd << std::endl;
  // Remove fd from epoll.
  recv_em_.RemoveAwaitingTask(fd);

  // Delete fd_message map record.
  {
    std::unique_lock<std::mutex> lock(map_mutex_);
    if (sessions_map_.find(fd) == sessions_map_.end()) {
      return;
    }
    delete sessions_map_[fd];
    sessions_map_.erase(sessions_map_.find(fd));
  }
}

void RpcServer::PrepareResponseData(RpcSession* session) {
  // Init a response header.
  RpcResponseHeader response_header;

  proto::SerializedMessage* sdres = nullptr;
  if (session->state() == RpcSession::RPC_METHOD_DONE) {
    // Serialize the response message
    sdres = session->rpc()->internal_response()->Serialize();

    // Set request size in request header
    response_header.set_rpc_response_length(sdres->size());
  }
  else {
    response_header.set_rpc_response_length(0);
  }

  // Set rpc return code and error message to rpc response header.
  response_header.set_rpc_return_code(session->rpc()->RpcReturnCode());
  response_header.set_rpc_return_msg(session->rpc()->RpcReturnMessage());

  // Serialize the request header
  proto::SerializedMessage* sdhdr = response_header.Serialize();

  // Send packet header:
  // | check number | header-length | request-length | ... data ... |
  //      4 byte          4 byte          4 byte
  int check_num = session->check_num();
  int res_header_size = sdhdr->size(); // response header size
  int res_size = sdres? sdres->size() : 0;  // user response size

  // Copy all data to internal buffer to prepare for sending.
  session->ResetBuf(3 * sizeof(int) + res_header_size + res_size);
  memcpy(session->InternalBuf(), (const char*)&check_num, sizeof(int));
  memcpy(session->InternalBuf() + sizeof(int), (const char*)&res_header_size,
         sizeof(int));
  memcpy(session->InternalBuf() + 2 * sizeof(int), (const char*)&res_size,
         sizeof(int));
  memcpy(session->InternalBuf() + 3 * sizeof(int),
         sdhdr->GetBytes(), sdhdr->size());
  if (sdres) {
    memcpy(session->InternalBuf() + 3 * sizeof(int) + sdhdr->size(),
           sdres->GetBytes(), sdres->size());
    delete sdres;
  }

  delete sdhdr;
}

int RpcServer::ParseRpcPacketHeader(RpcSession* session) {
  // Create rpc object for this session.
  Rpc* rpc = session->CreateRpc();

  if (!session || session->bufSize() != 3 * sizeof(int)) {
    return -1;
  }

  char* buf = session->InternalBuf();
  session->set_check_num(*(reinterpret_cast<int*>(buf)));
  session->set_req_header_size(*(reinterpret_cast<uint32*>(buf + sizeof(int))));
  session->set_req_size(*(reinterpret_cast<uint32*>(buf + 2 * sizeof(int))));

  if (session->req_header_size() <= 0 || session->req_size() <= 0) {
    std::cerr << "[ERROR] "
              << "reqeust header size = " << session->req_header_size() << ", "
              << "request body size = " << session->req_size() << std::endl;
    rpc->SetRpcReturnCode(RpcResponseHeader::INVALID_RPC_PKT_HEADER);
    return -1;
  }
  return 0;
}

int RpcServer::ParseRpcRequestHeader(RpcSession* session) {
  RpcRequestHeader* req_hdr = new RpcRequestHeader();

  req_hdr->DeSerialize(session->InternalBuf(), session->bufSize());
  // Verify rpc requst length with data in packet header.
  if (req_hdr->rpc_request_length() != session->req_size()) {
    delete req_hdr;
    session->rpc()->SetRpcReturnCode(RpcResponseHeader::REQ_LENG_MISMATCH);
    return -1;
  }

  // Find rpc handler from handler_map.
  std::string full_service_name =
      req_hdr->service_name() + "." + req_hdr->method_name();
  RpcHandler* handler = FindRpcHandler(full_service_name);
  if (!handler) {
    session->rpc()->SetRpcReturnCode(RpcResponseHeader::UNKNOWN_SERVICE);
    std::cerr << "ERROR: Can't find service_name "
              << full_service_name << std::endl;
    return -1;
  }
  session->set_handler(handler);

  // Check rpc options
  session->set_keep_alive(req_hdr->keep_alive());

  session->SetRequestHeader(req_hdr);
  return 0;
}

int RpcServer::ParseRpcRequest(RpcSession* session) {
  // Get rpc handler.
  RpcHandler* handler = session->handler();

  // Create rpc obj for this session.
  if (handler->request_prototype) {
    session->rpc()->set_internal_request(handler->request_prototype->New());
  }
  if (handler->response_prototype) {
    session->rpc()->set_internal_response(handler->response_prototype->New());
  }
  if (handler->stream_prototype) {
    session->rpc()->set_internal_stream(handler->stream_prototype->New());
  }

  // Deserialize the rpc request.
  session->rpc()->internal_request()->DeSerialize(session->InternalBuf(),
                                                  session->bufSize());
  // Okay, we're ready to execute the rpc method.
  return 0;
}

const RpcService* RpcServer::FindRpcService(const std::string& name) {
  if (service_map_.find(name) != service_map_.end()) {
    return service_map_.at(name);
  }
  return nullptr;
}

void RpcServer::RegisterService(const std::string& name, RpcService* service) {
  service_map_.emplace(name, service);
}

void RpcServer::DeRegisterService(const std::string& name) {
  auto it = service_map_.end();
  if ((it = service_map_.find(name)) != service_map_.end()) {
    service_map_.erase(it);
  }
}

void RpcServer::RegisterRpcHandler(const std::string& name,
                                   std::shared_ptr<RpcHandler> handler) {
  handler_map_[name] = handler;
}

void RpcServer::DeRegisterRpcHandler(const std::string& name) {
  auto it = handler_map_.find(name);
  if (it != handler_map_.end()) {
    handler_map_.erase(it);
  }
}

RpcHandler* RpcServer::FindRpcHandler(const std::string& name) {
  if (handler_map_.find(name) != handler_map_.end()) {
    return handler_map_.at(name).get();
  }
  return nullptr;
}

// ******************************* RpcSession ******************************* //
RpcSession::RpcSession(int fd) {
  // Create server channel
  fd_ = fd;
  channel_.reset(new RpcServerChannel(new Network::Socket(fd)));
  channel_->Initialize();

  // prepare to receive rpc packet header = 12 byts.
  bufSize_ = 3 * sizeof(int);
  buf_ = new char[bufSize_];
}

RpcSession::~RpcSession() {
  if (buf_) {
    delete[] buf_;
  }
  if (req_hdr_) {
    delete req_hdr_;
  }
  if (res_hdr_) {
    delete res_hdr_;
  }
}

void RpcSession::ResetBuf(int size) {
  //std::cout << "Resetting buf = " << size << std::endl;
  if (buf_) {
    delete[] buf_;
    buf_ = nullptr;
  }
  if (size > 0) {
    buf_ = new char[size];
  }
  bufSize_ = size;
  received_size_ = 0;
  sent_size_ = 0;\
}

Rpc* RpcSession::CreateRpc() {
  rpc_.reset(new Rpc());
  return rpc_.get();
}

void RpcSession::ResetAll() {
  bufSize_ = 3 * sizeof(int);
  buf_ = new char[bufSize_];
  received_size_ = 0;
  sent_size_ = 0;
}

void RpcSession::PrintInternalBuffer() const {
  for (int i = 0; i < received_size_; i++) {
    printf("0x%x ", buf_[i] & 0xff);
  }
  printf("\n");
}

}  // namespace RPC
