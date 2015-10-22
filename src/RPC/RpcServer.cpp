#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <assert.h>

#include "RpcServer.h"

const char* lineseperator = "*************************************************";

namespace RPC {

RpcServer::RpcServer(int port) : recv_em_(5), port_(port) {
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
  recv_em_.AwaitTermination();
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
  std::cout << "Read_Handler() ..." << std::endl;
  
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
                                         session->remain_size())) > 0) {
      session->add_received_size(nread);
      if (session->bufFull()) {
        // Done reading packet header. Parse the header and reset internal
        // buffer. And change state to PARSING_REQ_HDR.
        if (ParseRpcPacketHeader(session) < 0) {
          RemoveSession(fd);
          return;
        }
        session->ResetBuf(session->req_header_size());
        session->set_state(RpcSession::PARSING_REQ_HDR);
        break;
      }
    }
    if ((nread == 0 && !session->bufFull()) ||
        (errno != EAGAIN && errno != 0)) {
      RemoveSession(fd);
      return;
    }
  }
  
  // Read request header
  buf = session->InternalBuf();
  if (session->state() == RpcSession::PARSING_REQ_HDR) {
    while (!session->bufFull() &&
           (nread = channel->ReceiveData(buf + session->received_size(),
                                         session->remain_size())) > 0) {
      session->add_received_size(nread);
      if (session->bufFull()) {
        // Done reading rpc request header. Deserialize RpcRequestHeader and
        // do sanity check.
        if (ParseRpcRequestHeader(session) < 0) {
          RemoveSession(fd);
          return;
        }
        session->ResetBuf(session->req_size());
        session->set_state(RpcSession::PARSING_REQ);
        break;
      }
    }
    // Unexpected EOF or errno other than EAGAIN.
    if ((nread == 0 && !session->bufFull()) ||
        (errno != EAGAIN && errno != 0)) {
      RemoveSession(fd);
      return;
    }
  }

  // Read rpc request
  buf = session->InternalBuf();
  if (session->state() == RpcSession::PARSING_REQ) {
    while (!session->bufFull() &&
           (nread = channel->ReceiveData(buf + session->received_size(),
                                         session->remain_size())) > 0) {
      session->add_received_size(nread);
      if (session->bufFull()) {
        // Done reading rpc request. Deserialize request and process.
        if (ParseRpcRequest(session) < 0) {
          RemoveSession(fd);
          return;
        }
        session->ResetBuf(0);
        session->set_state(RpcSession::READ_DONE);
        break;
      }
    }
    // Unexpected EOF or errno other than EAGAIN.
    if ((nread == 0 && !session->bufFull()) ||
        (errno != EAGAIN && errno != 0)) {
      RemoveSession(fd);
      return;
    }
  }

  // Excute the rpc method
  // TODO: should submit it to another specific thread pool for rpc processing.
  if (session->state() == RpcSession::READ_DONE) {
    (*session->handler()->rpc_method)(session->rpc());
  }
  else {
    recv_em_.ModifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
        Base::NewCallBack(&RpcServer::ReadRpcRequestHandler, this, fd));
  }

  // if (session->state() == RpcSession::FINISHREADING) {
  //   //std::cout << "Change to writing awating for " << fd << std::endl;
  //   //event_manger_.RemoveTaskWaitingReadable(fd);
  //   event_manger_.ModifyTaskWaitingStatus(fd, EPOLLOUT | EPOLLONESHOT,
  //       Base::NewCallBack(&SimpleServer::WriteRequestHandler, this, fd));
  // }
}

void RpcServer::RemoveSession(int fd) {
  std::cout << "Removing session fd" << std::endl;
}

int RpcServer::ParseRpcPacketHeader(RpcSession* session) {
  if (!session || session->bufSize() != 3 * sizeof(int)) {
    return -1;
  }
  char* buf = session->InternalBuf();
  session->set_check_num(*(reinterpret_cast<int*>(buf)));
  session->set_req_header_size(*(reinterpret_cast<uint32*>(buf + sizeof(int))));
  session->set_req_size(*(reinterpret_cast<uint32*>(buf + 2 * sizeof(int))));
  printf("check_num = %d\n", session->check_num());
  printf("req_header_size = %d\n", session->req_header_size());
  printf("req_size = %d\n", session->req_size());
  
  if (session->req_header_size() <= 0 || session->req_size() <= 0) {
    return -1;
  }
  return 0;
}

int RpcServer::ParseRpcRequestHeader(RpcSession* session) {
  RpcRequestHeader* req_hdr = new RpcRequestHeader();

  req_hdr->DeSerialize(session->InternalBuf(), session->bufSize());
  // std::cout << "service_name = " << req_hdr->service_name() << std::endl;
  // std::cout << "method_name = " << req_hdr->method_name() << std::endl;
  // std::cout << "rpc_request_length = " << req_hdr->rpc_request_length() << std::endl;
  // Verify rpc requst length with data in packet header.
  if (req_hdr->rpc_request_length() != session->req_size()) {
    delete req_hdr;
    return -1;
  }

  // Find rpc handler.
  std::string full_service_name =
      req_hdr->service_name() + "." + req_hdr->method_name();
  RpcHandler* handler = FindRpcHandler(full_service_name);
  if (!handler) {
    std::cerr << "ERROR: Can't find service_name "
              << full_service_name << std::endl;
    return -1;
  }
  session->set_handler(handler);

  session->SetRequestHeader(req_hdr);
  return 0;
}

int RpcServer::ParseRpcRequest(RpcSession* session) {
  // Find handler
  RpcHandler* handler = session->handler();

  // Create rpc obj for this session
  Rpc* rpc = session->CreateRpc();
  if (handler->request_prototype) {
    rpc->set_internal_request(handler->request_prototype->New());
  }
  if (handler->response_prototype) {
    rpc->set_internal_response(handler->response_prototype->New());
  }
  if (handler->stream_prototype) {
    rpc->set_internal_stream(handler->stream_prototype->New());
  }
  // TODO: set rpc internal callback as FinishRpc() which sends reply to user.
  // rpc->set_cb_final();

  // Deserialize the rpc request
  rpc->internal_request()->DeSerialize(session->InternalBuf(),
                                       session->bufSize());
  // Okay, we're ready to execute the rpc method.
  return 0;
}

const RpcService* RpcServer::FindRpcService(std::string name) {
  if (service_map_.find(name) != service_map_.end()) {
    return service_map_.at(name);
  }
  return nullptr;
}

void RpcServer::RegisterService(std::string name, RpcService* service) {
  service_map_.emplace(name, service);
}

void RpcServer::DeRegisterService(std::string name) {
  auto it = service_map_.end();
  if ((it = service_map_.find(name)) != service_map_.end()) {
    service_map_.erase(it);
  }
}

RpcHandler* RpcServer::FindRpcHandler(std::string name) {
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
  std::cout << "Resetting buf = " << size << std::endl;
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

void RpcSession::PrintInternalBuffer() const {
  for (int i = 0; i < received_size_; i++) {
    printf("0x%x ", buf_[i] & 0xff);
  }
  printf("\n");
}

}  // namespace RPC
