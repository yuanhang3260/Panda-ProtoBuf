#ifndef RPC_SERVICE_
#define RPC_SERVICE_

#include <vector>
#include <map>

#include "../Utility/CallBack.h"
#include "RpcServer.h"

namespace RPC {

class RpcService {
 public:
  RpcService(const std::string name);
  virtual ~RpcService() {};

  // Get rpc service name.
  std::string name() const { return name_; }

  // rpc server accessor
  const RpcServer* rpc_server() const { return rpc_server_; }
  void set_rpc_server(RpcServer* rpc_server) { rpc_server_ = rpc_server; }

  // Register and de-register this rpc service to a rpc server.
  virtual void RegisterToServer() {};
  virtual void DeRegisterFromServer() {};

  // Register rpc handlers to a rpc server's handler map.
  virtual void InternalRegisterHandlers(RpcHandlerMap* handler_map) {};
  virtual void InternalDeRegisterHandlers(RpcHandlerMap* handler_map) {};

 protected:
  void UnInplemented(Rpc* rpc, Base::Closure* done);

  std::string name_;
  RpcServer* rpc_server_;
};

}  // namespace RPC

#endif  /* RPC_SERVICE_ */