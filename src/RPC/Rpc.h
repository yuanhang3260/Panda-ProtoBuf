#ifndef RPC_BASE
#define RPC_BASE

#include "../Proto/Message.h"
#include "../Utility/CallBack.h"

namespace RPC {

class Rpc {
 public:
  Rpc();
  virtual ~Rpc();

  const proto::Message* internal_request() const { return request_; }
  void set_internal_request(const proto::Message* req) { request_ = req; }

  proto::Message* internal_response() const { return response_; }
  void set_internal_response(proto::Message* res) { response_ = res; }

  proto::Message* internal_stream() const { return stream_; }
  void set_internal_stream(proto::Message* stream) { stream_ = stream; }

  Base::Closure* cb_final() const { return cb_final_; }
  void set_cb_final(Base::Closure* cb_final) { cb_final_ = cb_final; }

 private:
  const proto::Message* request_;
  proto::Message* response_;
  proto::Message* stream_;
  Base::Closure* cb_final_;
};

}  // namespace RPC

#endif  /* RPC_BASE */