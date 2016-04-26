#ifndef RPC_HANDLER
#define RPC_HANDLER

#include <functional>

#include "../Proto/Message.h"
#include "Rpc.h"

namespace RPC {

// InternalRpcMethod saved in RpcHandler maintained by RpcServer handler map.
// This function signature takes only one parameter: Rpc, which will be
// initialized when rpc server accepts a new rpc request packet and processes
// it. The most critical part of a Rpc object is the internal rpc request
// message and response messsage.
using InternalRpcMethod = std::function<void(Rpc*)>;

class RpcHandler {
 public:
  RpcHandler(std::string _fullname,
             std::string _method_name, 
             const proto::Message* _request_prototype,
             const proto::Message* _response_prototype,
             const proto::Message* _stream_prototype,
             InternalRpcMethod _rpc_method) :
      fullname(_fullname),
      method_name(_method_name),
      request_prototype(_request_prototype),
      response_prototype(_response_prototype),
      stream_prototype(_stream_prototype),
      rpc_method(_rpc_method) {}

  std::string fullname;
  std::string method_name;
  const proto::Message* request_prototype = nullptr;
  const proto::Message* response_prototype = nullptr;
  const proto::Message* stream_prototype = nullptr;
  InternalRpcMethod rpc_method;
  // TODO: maybe handler executor ?
};

}  // namespace RPC

#endif  /* RPC_HANDLER */