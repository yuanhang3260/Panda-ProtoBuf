#include "RpcClientChannel.h"

namespace RPC {

RpcClientChannel::RpcClientChannel(std::string hostname, int port) :
    hostname_(hostname),
    port_(port) {
  socket_.reset(new net::ClientSocket(hostname_, port_));
}

int RpcClientChannel::ConnectToServer() {
  if ((dynamic_cast<net::ClientSocket*>(socket_.get()))->Connect() == 0) {
    connected_ = true;
    return 0;
  }
  return -1;
}

}  // namespace RPC
