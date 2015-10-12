#ifndef RPC_SERVICE_
#define RPC_SERVICE_

#include <vector>
#include <map>

#include "RpcServer.h"

namespace RPC {

class RpcService {
 public:
  RpcService(const std::string& name);
  virtual ~RpcService();

  std::string name() const { return name_; }

  const RpcServer* rpc_server() const { return rpc_server_; }
  void set_rpc_server(RpcServer* rpc_server) { rpc_server_ = rpc_server; }

 private:
  std::string name_;
  RpcServer* rpc_server_;
};

}  // namespace RPC

#endif  /* RPC_SERVICE_ */