#ifndef RPC_CHANNEL_BASE_
#define RPC_CHANNEL_BASE_

#include <string>
#include <memory>

#include "Network/Socket.h"
#include "Utility/BufferedDataReader.h"
#include "Utility/BufferedDataWriter.h"

namespace RPC {

class RpcChannelBase {
 public:
  RpcChannelBase() = default;
  RpcChannelBase(net::Socket* socket);
  RpcChannelBase(const RpcChannelBase&) = delete;
  RpcChannelBase& operator=(const RpcChannelBase&) = delete;

  void Initialize();
  bool IsReady() const;
  net::Socket* socket() { return socket_.get(); } 

  int ReceiveData(char* buf, unsigned int size);
  int SendData(const char* buf, unsigned int size);
  void FlushSend();

  bool connected() const { return connected_; }
  void Disconnect();

 protected:
  std::string hostname_;
  int port_;
  std::unique_ptr<net::Socket> socket_;
  std::unique_ptr<Utility::BufferedDataReader> sock_reader_;
  std::unique_ptr<Utility::BufferedDataWriter> sock_writer_;

  bool connected_ = false;
};

}  // namespace RPC

#endif  /* RPC_CHANNEL_BASE_ */