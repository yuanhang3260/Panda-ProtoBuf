#include "RpcServerChannel.h"

namespace RPC {

RpcServerChannel::RpcServerChannel(net::Socket* socket) :
    RpcChannelBase(socket) {
}

}  // namespace RPC
