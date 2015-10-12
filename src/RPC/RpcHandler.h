#ifndef RPC_HANDLER
#define RPC_HANDLER

#include <functional>

#include "../Proto/Message.h"
#include "Rpc.h"

namespace RPC {

using InternalRpcMethod = std::function<void(Rpc*)>;

struct RpcHandler {
  std::string fullname;
  std::string method_name;
  proto::Message* request_prototype;
  proto::Message* response_prototype;
  proto::Message* stream_prototype;
  InternalRpcMethod rpc_method;
  // TODO: maybe handler executor ?
};

}  // namespace RPC

#endif  /* RPC_HANDLER */