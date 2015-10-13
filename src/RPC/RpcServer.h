#ifndef RPC_SERVER
#define RPC_SERVER

#include <vector>
#include <map>
#include <memory>

#include "RpcHandler.h"

namespace RPC {

using RpcHandlerMap = std::map<std::string, std::shared_ptr<RpcHandler>>;

class RpcServer {
 public:
  RpcServer(int port);
  virtual ~RpcServer();

  // Get internal handler_map.
  RpcHandlerMap* handler_map() { return &handler_map_; }

  // Look up RpcHandler by fullname. Returns nullptr if non-existent.
  RpcHandler* FindRpcHandler(std::string name);

 private:
  RpcHandlerMap handler_map_;
  int port_;
};

}  // namespace RPC

#endif  /* RPC_SERVER */
