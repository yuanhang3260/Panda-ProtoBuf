#include "RpcChannelBase.h"

namespace RPC {

RpcChannelBase::RpcChannelBase(net::Socket* socket) {
  socket_.reset(socket);
}

void RpcChannelBase::Initialize() {
  if (!sock_reader_) {
    sock_reader_.reset(new Utility::BufferedDataReader(socket_.get()));
  }
  sock_reader_->Reset();

  if (!sock_writer_) {
    sock_writer_.reset(new Utility::BufferedDataWriter(socket_.get()));
  }
  sock_writer_->Reset();
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

  int nread = sock_reader_->Read(buf, 0, size);
  // for (int i = 0; i < nread; i++) {
  //   printf("buf[%d] = 0x%x\n", i, buf[i] & 0xff);
  // }
  // printf("channel nread = %d\n", nread);
  return nread;
}

void RpcChannelBase::FlushSend() {
  if (!socket_ || !sock_writer_) {
    return;
  }
  sock_writer_->Flush();
}

void RpcChannelBase::Disconnect() {
  socket_->Close();
  //perror("");
  connected_ = false;
}

}  // namespace RPC