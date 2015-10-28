#include "RpcServerChannel.h"

namespace RPC {

RpcServerChannel::RpcServerChannel(Network::Socket* socket) :
    RpcChannelBase(socket) {
}

}  // namespace RPC
