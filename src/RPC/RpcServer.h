#ifndef RPC_SERVER
#define RPC_SERVER

#include <vector>
#include <map>
#include <memory>
#include <mutex>

#include "Network/Socket.h"
#include "Utility/EventManager.h"
#include "RpcHandler.h"
#include "RpcService.h"
#include "RpcServerChannel.h"
#include "RpcPacket_pb.h"

namespace RPC {

class RpcService;
class RpcSession;

using RpcServiceMap = std::map<std::string, RpcService*>;
using RpcHandlerMap = std::map<std::string, std::shared_ptr<RpcHandler>>;

class RpcServer {
 public:
  RpcServer(int port);
  RpcServer(int port, int recv_em_size);
  virtual ~RpcServer() {};

  void StartServing();

  // Get Service Map
  RpcServiceMap* service_map() { return &service_map_; }
  // Find Service
  const RpcService* FindRpcService(const std::string& name);
  // Register service
  void RegisterService(const std::string& name, RpcService* service);
  // DeRegister service
  void DeRegisterService(const std::string& name);
  // Register rpc handler
  void RegisterRpcHandler(const std::string& name, std::shared_ptr<RpcHandler>);
  // DeRegister rpc handler.
  void DeRegisterRpcHandler(const std::string& name);

  // Get internal handler_map.
  RpcHandlerMap* handler_map() { return &handler_map_; }
  // Look up RpcHandler by fullname. Returns nullptr if non-existent.
  RpcHandler* FindRpcHandler(const std::string& name);

 private:
  void RpcConnectionListenerHandler();
  void ReadRpcRequestHandler(int fd);
  void WriteRpcResponseHandler(int fd);

  int ParseRpcPacketHeader(RpcSession* session);
  int ParseRpcRequestHeader(RpcSession* session);
  int ParseRpcRequest(RpcSession* session);

  void EnqueueRpcBanckendProcessing(RpcSession* session);
  void BackendRpcProcess(RpcSession* session);
  void PrepareResponseData(RpcSession* session);

  void RemoveSession(int fd);

  // Rpc service map
  RpcServiceMap service_map_;
  // Rpc handlers
  RpcHandlerMap handler_map_;

  // All active RpcSessions
  std::map<int, RpcSession*> sessions_map_;
  std::mutex map_mutex_;

  // Thread executors
  Executors::EventManager recv_em_; // Incoming request processor 
  Executors::EventManager run_em_; // Execute rpc and reply

  // Listening socket
  int port_;
  std::shared_ptr<net::Socket> listen_socket_;
};

class RpcSession {
 public:
  enum RpcState {
    INIT = 0,
    PARSING_PKT_HDR = 1,
    PARSING_REQ_HDR = 2,
    PARSING_REQ = 3,
    READ_DONE = 4,
    RPC_METHOD_DONE = 5,
    REQUEST_ERROR = 6,
    WRITING_SUCCESS_RES = 7,
    WRITING_ERROR_RES = 7,
    WRITE_DONE = 8,
  };

  RpcSession(int fd);
  ~RpcSession();

  // socket
  int getFd() const { return fd_; }
  net::Socket* socket() { return channel_->socket(); }
  RpcServerChannel* channel() { return channel_.get(); }

  // rpc packet meta
  int check_num() const { return check_num_; }
  void set_check_num(int check_num) { check_num_ = check_num; }
  uint32 req_header_size() const { return req_header_size_; }
  void set_req_header_size(uint32 size) { req_header_size_ = size; }
  uint32 req_size() const { return req_size_; }
  void set_req_size(uint32 size) { req_size_ = size; }

  // rpc headers
  RpcRequestHeader* RequestHeader() { return req_hdr_; };
  void SetRequestHeader(RpcRequestHeader* header) { req_hdr_ = header; }
  RpcResponseHeader* ResponseHeader() { return res_hdr_; }
  void SetResponseHeader(RpcResponseHeader* header) { res_hdr_ = header; }

  // reset internal buf
  char* InternalBuf() { return buf_; }
  void ResetBuf(int size);
  int bufSize() const { return bufSize_; }
  int received_size() const { return received_size_; }
  void set_received_size(int size) { received_size_ = size; }
  void add_received_size(int size) { received_size_ += size; }
  int remain_size_to_recv() const { return bufSize_ - received_size_; }
  int remain_size_to_send() const { return bufSize_ - sent_size_; }
  bool bufFull() const { return bufSize_ == received_size_; }
  int sent_size() const { return sent_size_; }
  void set_sent_size(int size) { sent_size_ = size; }
  void add_sent_size(int size) { sent_size_ += size; }

  // rpc state
  RpcState state() const { return state_; }
  void set_state(RpcState state) { state_ = state; }

  RpcHandler* handler() const { return handler_; }
  void set_handler(RpcHandler* handler) { handler_ = handler; }

  bool keep_alive() const { return keep_alive_; }
  void set_keep_alive(bool ka) { keep_alive_ = ka; }

  void ResetAll();

  // RPC obj
  Rpc* rpc() { return rpc_.get(); }
  Rpc* CreateRpc();

  // Helper functions
  void PrintInternalBuffer() const;

 private:
  int fd_;
  std::unique_ptr<RpcServerChannel> channel_;

  int check_num_ = -1;
  uint32 req_header_size_ = -1;
  uint32 req_size_ = -1;
  RpcRequestHeader* req_hdr_ = nullptr;
  RpcResponseHeader* res_hdr_ = nullptr;
  
  char* buf_ = nullptr;
  int bufSize_ = 0;
  int received_size_ = 0;
  int sent_size_ = 0;

  RpcState state_ = INIT;
  RpcHandler* handler_;
  std::unique_ptr<Rpc> rpc_;

  bool keep_alive_ = false;
};

}  // namespace RPC

#endif  /* RPC_SERVER */
