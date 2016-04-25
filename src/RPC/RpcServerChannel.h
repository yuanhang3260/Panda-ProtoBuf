#ifndef RPC_SERVER_CHANNEL_
#define RPC_SERVER_CHANNEL_

#include <string>
#include <memory>

#include "Network/Socket.h"
#include "RpcChannelBase.h"

namespace RPC {

class RpcServerChannel : public RpcChannelBase {
 public:
  RpcServerChannel() = default;
  RpcServerChannel(Network::Socket* socket);
  RpcServerChannel(const RpcServerChannel&) = delete;
  RpcServerChannel& operator=(const RpcServerChannel&) = delete;

 private:
};

}  // namespace RPC

#endif  /* RPC_SERVER_CHANNEL_ */