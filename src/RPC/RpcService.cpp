#include "RpcService.h"

namespace RPC {

RpcService::RpcService(const std::string& name) : name_(name) {
}

void RpcService::UnInplemented(Rpc* rpc, Base::Closure* done) {
  (void)rpc;
  if (done) {
    done->Run();
  }
}

}  // namespace RPC
