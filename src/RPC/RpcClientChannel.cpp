#include "RpcClientChannel.h"

namespace RPC {

RpcClientChannel::RpcClientChannel(std::string hostname, int port) :
    hostname_(hostname),
    port_(port) {
  socket_.reset(Network::Socket::CreateClientSocket(hostname_, port_));
}

int RpcClientChannel::ConnectToServer() {
  if (socket_->ClientConnect() == 0) {
    connected_ = true;
    return 0;
  }
  return -1;
}

}  // namespace RPC
