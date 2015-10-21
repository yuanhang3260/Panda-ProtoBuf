#include "RpcChannelBase.h"

namespace RPC {

RpcChannelBase::RpcChannelBase(Network::Socket* socket) {
  socket_.reset(socket);
}

void RpcChannelBase::Initialize() {
  sock_reader_.reset(new Utility::BufferedDataReader(socket_.get()));
  sock_writer_.reset(new Utility::BufferedDataWriter(socket_.get()));
}

bool RpcChannelBase::IsReady() const {
  return socket_ && sock_reader_ && sock_writer_ &&
         !socket_->closed();
}

int RpcChannelBase::SendData(const char* buf, unsigned int size) {
  if (!socket_ || !sock_writer_) {
    return -1;
  }
  return sock_writer_->Write(buf, 0, size);
}

int RpcChannelBase::ReceiveData(char* buf, unsigned int size) {
  if (!socket_ || !sock_reader_) {
    return -1;
  }
  return sock_reader_->Read(buf, 0, size);
}

void RpcChannelBase::FlushSend() {
  if (!socket_ || !sock_writer_) {
    return;
  }
  sock_writer_->Flush();
}

}  // namespace RPC