#include "Rpc.h"

namespace RPC {

Rpc::~Rpc() {
  if (request_) delete request_;
  if (response_) delete response_;
  if (stream_) delete stream_;
  if (cb_final_) delete cb_final_;
}

}  // namespace RPC