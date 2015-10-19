#ifndef RPC_CLIENT_CHANNEL_
#define RPC_CLIENT_CHANNEL_

#include <string>
#include <memory>

#include "Network/Socket.h"
#include "Utility/BufferedDataReader.h"
#include "RpcChannelBase.h"
#include "RpcSession_pb.h"

namespace RPC {

class RpcClientChannel : public RpcChannelBase {
 public:
  RpcClientChannel(std::string hostname, int port);
  RpcClientChannel(const RpcClientChannel&) = delete;
  RpcClientChannel& operator=(const RpcClientChannel&) = delete;

 private:
  std::string hostname_;
  int port_;
};

}  // namespace RPC

#endif  /* RPC_CLIENT_CHANNEL_ */