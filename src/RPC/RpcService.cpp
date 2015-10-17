#include "RpcService.h"

namespace RPC {

RpcService::RpcService(const std::string name) : name_(name) {
}

RpcService::~RpcService() {
}

// ----------------------------- Server methods ----------------------------- //
void RpcService::UnInplemented(Rpc* rpc, Base::Closure* done) {
  (void)rpc;
  if (done) {
    done->Run();
  }
}

// ----------------------------- Client methods ----------------------------- //
void RpcService::InitStub(std::string name,
                          ::RPC::RpcClientChannel* channel,
                          const ::RPC::RpcStubOptions options) {
  name_ = name;
  rpc_client_channel_ = channel;
  options_ = options;
}

void RpcService::StartClientRpcCall(
    Rpc* rpc,
    const RpcDescriptor* descriptor,
    const proto::Message* request,
    proto::Message* response,
    Base::Closure* cb) {

}

}  // namespace RPC
