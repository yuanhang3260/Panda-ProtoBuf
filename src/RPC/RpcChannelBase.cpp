#include "RpcChannelBase.h"

namespace RPC {

RpcChannelBase::RpcChannelBase(Network::Socket* socket) {
  socket_.reset(socket);
}

}  // namespace RPC