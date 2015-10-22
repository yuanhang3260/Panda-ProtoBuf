#ifndef RPCSESSION_PB_H_
#define RPCSESSION_PB_H_

#include <string>
#include <vector>

#include "Proto/Message.h"
#include "Proto/RepeatedFields.h"
#include "Proto/SerializedMessage.h"

void static_init_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
void static_init_default_instances_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();

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

 private:
  // has bits
  char has_bits_[1];
  // message fields
  std::string service_name_ = "";
  std::string method_name_ = "";
  unsigned int rpc_request_length_ = 0;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static RpcRequestHeader* default_instance_;

  friend void ::static_init_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
  friend void ::static_init_default_instances_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
};

class RpcResponseHeader: public ::proto::Message {
 public:
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

  // --- Field accessors --- //

  // "rpc_response_length" = 1
  bool has_rpc_response_length() const;
  unsigned int rpc_response_length() const;
  void set_rpc_response_length(unsigned int rpc_response_length);
  void clear_rpc_response_length();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  unsigned int rpc_response_length_ = 0;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static RpcResponseHeader* default_instance_;

  friend void ::static_init_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
  friend void ::static_init_default_instances_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
};

}  // namespace RPC


#endif  /* RPCSESSION_PB_H_ */
