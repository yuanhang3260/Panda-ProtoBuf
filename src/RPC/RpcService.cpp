#include "RpcService.h"

namespace RPC {

RpcService::RpcService(const std::string name) : name_(name) {
}

RpcService::~RpcService() {
}

void RpcService::UnInplemented(Rpc* rpc, Base::Closure* done) {
  (void)rpc;
  if (done) {
    done->Run();
  }
}

void RpcService::StartClientRpcCall(
    Rpc* rpc,
    const RpcDescriptor* descriptor,
    const proto::Message* request,
    proto::Message* response,
    Base::Closure* cb) {

}

}  // namespace RPC
