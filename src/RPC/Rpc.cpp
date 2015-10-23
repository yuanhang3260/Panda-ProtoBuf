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

std::string Rpc::RpcCallStatus() {
  if (client_status_ == INIT) {
    return "Init";
  }
  else if (client_status_ == SUCCESS) {
    return "Success";
  }
  else if (client_status_ == INTERNAL_CHANNEL_ERROR) {
    return "Internal Channel Error";
  }
  else if (client_status_ == CHECK_NUM_ERROR) {
    return "check Num Error";
  }
  else if (client_status_ == RESPONSE_HEADER_SIZE_ERROR) {
    return "Response Header Size Error";
  }
  else if (client_status_ == RESPONSE_SIZE_ERROR) {
    return "Response Size Error";
  }
  else if (client_status_ == BAD_RESPONSE_HEADER) {
    return "Bad Response Header";
  }
  else {
    return "Unknown";
  }
}

}  // namespace RPC