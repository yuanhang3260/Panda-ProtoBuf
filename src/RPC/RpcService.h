#ifndef RPC_SERVICE_
#define RPC_SERVICE_

#include <vector>
#include <map>

#include "Utility/CallBack.h"
#include "Proto/Message.h"
#include "Rpc.h"
#include "RpcCommon.h"
#include "RpcServer.h"
#include "RpcClientChannel.h"

namespace RPC {

class RpcServer;
class RpcService;

using RpcHandlerMap = std::map<std::string, std::shared_ptr<RpcHandler>>;

class RpcService {
 public:
  RpcService() = default;
  RpcService(const std::string name);
  virtual ~RpcService();

  // Get rpc service name.
  std::string name() const { return name_; }

  // --------------------------- Server methods ----------------------------- //
  // rpc server accessor
  const RpcServer* rpc_server() const { return rpc_server_; }
  void set_rpc_server(RpcServer* rpc_server) { rpc_server_ = rpc_server; }

  // rpc descriptor
  virtual const RpcDescriptor* descriptor() = 0;

  // Register and de-register this rpc service to a rpc server.
  virtual void RegisterToServer() {};
  virtual void DeRegisterFromServer() {};

  // Register rpc handlers to a rpc server's handler map.
  virtual void InternalRegisterHandlers(RpcHandlerMap* handler_map) {};
  virtual void InternalDeRegisterHandlers(RpcHandlerMap* handler_map) {};

  // ---------------------------- Client methods ---------------------------- //
  void InitStub(std::string name,
                ::RPC::RpcClientChannel* channel,
                const ::RPC::RpcStubOptions options);

 protected:
  // ---------------------------- Server methods ---------------------------- //
  // Default server rpc method.
  void UnInplemented(Rpc* rpc, Base::Closure* done);

  // ---------------------------- Client methods ---------------------------- //
  // Client start call rpc.
  void StartClientRpcCall(Rpc* rpc,
                          const RpcDescriptor* descriptor,
                          std::string method_name,
                          const proto::Message* request,
                          proto::Message* response,
                          Base::Closure* cb);

  void DoRpcCall(Rpc* rpc,
                 const RpcDescriptor* descriptor,
                 std::string method_name,
                 const proto::Message* request,
                 proto::Message* response,
                 Base::Closure* cb);

  int ClientSendRequest(Rpc* rpc,
                        const RpcDescriptor* descriptor,
                        std::string method_name,
                        const proto::Message* request);

  int ClientReceiveResponse(Rpc* rpc,
                            proto::Message* response,
                            Base::Closure* cb);

  std::string name_;
  // server
  RpcServer* rpc_server_ = nullptr;
  // client
  std::unique_ptr<RpcClientChannel> rpc_client_channel_;
  ::RPC::RpcStubOptions options_;
};

}  // namespace RPC

#endif  /* RPC_SERVICE_ */