#ifndef RPC_HANDLER
#define RPC_HANDLER

#include <functional>

#include "../Proto/Message.h"
#include "Rpc.h"

namespace RPC {

using InternalRpcMethod = std::function<void(Rpc*)>;

class RpcHandler {
 public:
  RpcHandler(std::string _fullname,
             std::string _method_name, 
             proto::Message* _request_prototype,
             proto::Message* _response_prototype,
             proto::Message* _stream_prototype,
             InternalRpcMethod* _rpc_method) :
      fullname(_fullname),
      method_name(_method_name),
      request_prototype(_request_prototype),
      response_prototype(_response_prototype),
      stream_prototype(_stream_prototype),
      rpc_method(_rpc_method) {}

  ~RpcHandler() {
    if (request_prototype) delete request_prototype;
    if (response_prototype) delete response_prototype;
    if (stream_prototype) delete stream_prototype;
    if (rpc_method) delete rpc_method;
  }

  std::string fullname;
  std::string method_name;
  proto::Message* request_prototype;
  proto::Message* response_prototype;
  proto::Message* stream_prototype;
  InternalRpcMethod* rpc_method;
  // TODO: maybe handler executor ?
};

}  // namespace RPC

#endif  /* RPC_HANDLER */