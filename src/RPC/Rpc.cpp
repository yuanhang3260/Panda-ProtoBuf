#include "Rpc.h"

namespace RPC {

Rpc::~Rpc() {
  if (request_) delete request_;
  if (response_) delete response_;
  if (stream_) delete stream_;
  if (cb_final_) delete cb_final_;
}

void Rpc::Wait() {
  std::unique_lock<std::mutex> lock(rpc_wait_mutex_);
  rpc_wait_cond_.wait(lock, [this] { return rpc_finished_; });
}

void Rpc::SetRpcFinished() {
  rpc_wait_cond_.notify_one();

  std::unique_lock<std::mutex> lock(rpc_wait_mutex_);
  rpc_finished_ = true;
}

}  // namespace RPC