#ifndef RPC_SERVER
#define RPC_SERVER

#include <vector>
#include <map>
#include <memory>

#include "RpcHandler.h"

namespace RPC {

class RpcServer {
 public:
  RpcServer(int port);
  virtual ~RpcServer();

 private:
  std::map<std::string, std::shared_ptr<RpcHandler>> handler_map_;
  int port_;
};

}  // namespace RPC

#endif  /* RPC_SERVER */