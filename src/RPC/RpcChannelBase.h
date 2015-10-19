#ifndef RPC_CHANNEL_BASE_
#define RPC_CHANNEL_BASE_

#include <string>
#include <memory>

#include "Network/Socket.h"
#include "Utility/BufferedDataReader.h"
#include "RpcSession_pb.h"

namespace RPC {

class RpcChannelBase {
 public:
  RpcChannelBase() = default;
  RpcChannelBase(Network::Socket* socket);
  RpcChannelBase(const RpcChannelBase&) = delete;
  RpcChannelBase& operator=(const RpcChannelBase&) = delete;

 protected:
  std::string hostname_;
  int port_;
  std::unique_ptr<Network::Socket> socket_;
  std::unique_ptr<Utility::BufferedDataReader> sock_reader_;
  std::unique_ptr<Utility::BufferedDataReader> sock_writer_;
};

}  // namespace RPC

#endif  /* RPC_CHANNEL_BASE_ */