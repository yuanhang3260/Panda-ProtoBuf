#ifndef RPC_CLIENT_CHANNEL_
#define RPC_CLIENT_CHANNEL_

#include <string>

namespace RPC {

class RpcClientChannel {
 public:
  RpcClientChannel(std::string hostname, int port);

 private:
  std::string hostname_;
  int port_;
};

}  // namespace RPC

#endif  /* RPC_CLIENT_CHANNEL_ */