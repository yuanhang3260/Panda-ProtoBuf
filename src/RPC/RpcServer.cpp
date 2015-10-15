#include "RpcServer.h"

namespace RPC {

RpcServer::RpcServer(int port) : port_(port) {
}

RpcHandler* RpcServer::FindRpcHandler(std::string name) {
  if (handler_map_.find(name) != handler_map_.end()) {
    return handler_map_[name].get();
  }
  return nullptr;
}

void RpcServer::StartRpcCall(Rpc* rpc) {
  // TODO
}

}  // namespace RPC
