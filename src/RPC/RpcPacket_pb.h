#ifndef RPCPACKET_PB_H_
#define RPCPACKET_PB_H_

#include <string>
#include <vector>

#include "Proto/Message.h"
#include "Proto/RepeatedFields.h"
#include "Proto/SerializedMessage.h"

void static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_src_RPC_RpcPacket();
void static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_src_RPC_RpcPacket();

namespace RPC {

class RpcRequestHeader: public ::proto::Message {
 public:
  // constructors and destructor //
  RpcRequestHeader();
  ~RpcRequestHeader();
  RpcRequestHeader(const RpcRequestHeader& other);  // copy constructor
  RpcRequestHeader(RpcRequestHeader&& other);  // move constructor
  RpcRequestHeader& operator=(const RpcRequestHeader& other); // copy assignment
  RpcRequestHeader& operator=(RpcRequestHeader&& other);  // move assignment
  void Swap(RpcRequestHeader* other);  // Swap
  ::proto::Message* New() const override;  // New()
  void CopyFrom(const RpcRequestHeader& other);  // CopyFrom()
  void MoveFrom(RpcRequestHeader&& other);  // MoveFrom()
  bool Equals(const RpcRequestHeader& other) const;  // Compare
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const RpcRequestHeader& default_instance();
  void Print(int indent_num=0) const override;

  // --- Field accessors --- //

  // "service_name" = 1
  bool has_service_name() const;
  const std::string& service_name() const;
  void set_service_name(const std::string& service_name);
  void set_service_name(const char* service_name);
  void set_service_name(const char* service_name, int size);
  std::string mutable_service_name();
  void clear_service_name();

  // "method_name" = 2
  bool has_method_name() const;
  const std::string& method_name() const;
  void set_method_name(const std::string& method_name);
  void set_method_name(const char* method_name);
  void set_method_name(const char* method_name, int size);
  std::string mutable_method_name();
  void clear_method_name();

  // "rpc_request_length" = 3
  bool has_rpc_request_length() const;
  unsigned int rpc_request_length() const;
  void set_rpc_request_length(unsigned int rpc_request_length);
  void clear_rpc_request_length();

  // "keep_alive" = 4
  bool has_keep_alive() const;
  bool keep_alive() const;
  void set_keep_alive(bool keep_alive);
  void clear_keep_alive();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  std::string service_name_ = "";
  std::string method_name_ = "";
  unsigned int rpc_request_length_ = 0;
  bool keep_alive_ = false;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static RpcRequestHeader* default_instance_;

  friend void ::static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_src_RPC_RpcPacket();
  friend void ::static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_src_RPC_RpcPacket();
};

class RpcResponseHeader: public ::proto::Message {
 public:
  enum RpcReturnCode {
    OK,
    INVALID_RPC_PKT_HEADER,
    REQ_LENG_MISMATCH,
    UNKNOWN_SERVICE,
    INTERNAL_SERVER_ERROR,
    NONE,
  };

  // constructors and destructor //
  RpcResponseHeader();
  ~RpcResponseHeader();
  RpcResponseHeader(const RpcResponseHeader& other);  // copy constructor
  RpcResponseHeader(RpcResponseHeader&& other);  // move constructor
  RpcResponseHeader& operator=(const RpcResponseHeader& other); // copy assignment
  RpcResponseHeader& operator=(RpcResponseHeader&& other);  // move assignment
  void Swap(RpcResponseHeader* other);  // Swap
  ::proto::Message* New() const override;  // New()
  void CopyFrom(const RpcResponseHeader& other);  // CopyFrom()
  void MoveFrom(RpcResponseHeader&& other);  // MoveFrom()
  bool Equals(const RpcResponseHeader& other) const;  // Compare
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const RpcResponseHeader& default_instance();
  void Print(int indent_num=0) const override;

  // --- Field accessors --- //

  // "rpc_return_code" = 1
  bool has_rpc_return_code() const;
  RpcResponseHeader::RpcReturnCode rpc_return_code() const;
  void set_rpc_return_code(RpcResponseHeader::RpcReturnCode rpc_return_code);
  void clear_rpc_return_code();

  // "rpc_return_msg" = 2
  bool has_rpc_return_msg() const;
  const std::string& rpc_return_msg() const;
  void set_rpc_return_msg(const std::string& rpc_return_msg);
  void set_rpc_return_msg(const char* rpc_return_msg);
  void set_rpc_return_msg(const char* rpc_return_msg, int size);
  std::string mutable_rpc_return_msg();
  void clear_rpc_return_msg();

  // "rpc_response_length" = 3
  bool has_rpc_response_length() const;
  unsigned int rpc_response_length() const;
  void set_rpc_response_length(unsigned int rpc_response_length);
  void clear_rpc_response_length();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  RpcResponseHeader::RpcReturnCode rpc_return_code_ = RpcResponseHeader::NONE;
  std::string rpc_return_msg_ = "";
  unsigned int rpc_response_length_ = 0;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static RpcResponseHeader* default_instance_;

  friend void ::static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_src_RPC_RpcPacket();
  friend void ::static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_src_RPC_RpcPacket();
};

}  // namespace RPC


#endif  /* RPCPACKET_PB_H_ */
