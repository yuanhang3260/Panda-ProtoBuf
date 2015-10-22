#include "RpcClientChannel.h"

namespace RPC {

RpcClientChannel::RpcClientChannel(std::string hostname, int port) :
    hostname_(hostname),
    port_(port) {
  socket_.reset(Network::Socket::CreateClientSocket(hostname_, port_));
}

}  // namespace RPC
