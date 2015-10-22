#include <memory>
#include <mutex>
#include <map>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "RpcSession_pb.h"

namespace {

std::shared_ptr<::proto::ProtoParser::Message> RpcRequestHeader_descriptor_;
std::shared_ptr<::proto::MessageReflection> RpcRequestHeader_reflection_;
std::shared_ptr<::proto::ProtoParser::Message> RpcResponseHeader_descriptor_;
std::shared_ptr<::proto::MessageReflection> RpcResponseHeader_reflection_;

}  // namepsace

void static_init_default_instances_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession() {
  if (RPC::RpcRequestHeader::default_instance_ == NULL) {
    RPC::RpcRequestHeader::default_instance_ = new RPC::RpcRequestHeader();
    RPC::RpcRequestHeader::default_instance_->InitAsDefaultInstance();
  }
  if (RPC::RpcResponseHeader::default_instance_ == NULL) {
    RPC::RpcResponseHeader::default_instance_ = new RPC::RpcResponseHeader();
    RPC::RpcResponseHeader::default_instance_->InitAsDefaultInstance();
  }
}

void static_init_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  ::proto::ProtoParser::ProtoParser parser(
      ::proto::ProtoParser::CPP,
      "/home/hy/Desktop/Snoopy-ProtoBuf/src/RPC/RpcSession.proto");
  CHECK(parser.ParseProto(),
        "static class initialization for /home/hy/Desktop/Snoopy-ProtoBuf/src/RPC/RpcSession.proto failed");

  static_init_default_instances_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();

  int i = 0;
  // static init for class RpcRequestHeader
  static const int RpcRequestHeader_offsets_[3] = {
    PROTO_MESSAGE_FIELD_OFFSET(RPC::RpcRequestHeader, service_name_),
    PROTO_MESSAGE_FIELD_OFFSET(RPC::RpcRequestHeader, method_name_),
    PROTO_MESSAGE_FIELD_OFFSET(RPC::RpcRequestHeader, rpc_request_length_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[0]->mutable_fields_list()) {
    field->set_field_offset(RpcRequestHeader_offsets_[i++]);
  }
  RpcRequestHeader_descriptor_ = parser.mutable_messages_list()[0];
  RpcRequestHeader_reflection_.reset(
      new ::proto::MessageReflection(
          RpcRequestHeader_descriptor_,
          RPC::RpcRequestHeader::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(RPC::RpcRequestHeader, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(RpcRequestHeader_reflection_);

  // static init for class RpcResponseHeader
  static const int RpcResponseHeader_offsets_[1] = {
    PROTO_MESSAGE_FIELD_OFFSET(RPC::RpcResponseHeader, rpc_response_length_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[1]->mutable_fields_list()) {
    field->set_field_offset(RpcResponseHeader_offsets_[i++]);
  }
  RpcResponseHeader_descriptor_ = parser.mutable_messages_list()[1];
  RpcResponseHeader_reflection_.reset(
      new ::proto::MessageReflection(
          RpcResponseHeader_descriptor_,
          RPC::RpcResponseHeader::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(RPC::RpcResponseHeader, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(RpcResponseHeader_reflection_);

}

// Force static_init_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession() to be called at initialization time.
struct static_init_forcer_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession {
  static_init_forcer_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession() {
    static_init_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
  }
} static_init_forcer_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession_obj_;


namespace RPC {

// ******************** RpcRequestHeader ******************** //
// constructor
RpcRequestHeader::RpcRequestHeader() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
RpcRequestHeader::RpcRequestHeader(const RpcRequestHeader& other) {
  CopyFrom(other);
}

// move constructor
RpcRequestHeader::RpcRequestHeader(RpcRequestHeader&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
RpcRequestHeader& RpcRequestHeader::operator=(const RpcRequestHeader& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
RpcRequestHeader& RpcRequestHeader::operator=(RpcRequestHeader&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* RpcRequestHeader::New() const {
  return reinterpret_cast<::proto::Message*>(new RpcRequestHeader());
}

// CopyFrom()
void RpcRequestHeader::CopyFrom(const RpcRequestHeader& other) {
  service_name_ = other.service_name();
  method_name_ = other.method_name();
  rpc_request_length_ = other.rpc_request_length();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void RpcRequestHeader::MoveFrom(RpcRequestHeader&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  service_name_ = std::move(other.mutable_service_name());
  method_name_ = std::move(other.mutable_method_name());
  rpc_request_length_ = other.rpc_request_length();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool RpcRequestHeader::Equals(const RpcRequestHeader& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (service_name_ != other.service_name_) {
    return false;
  }
  if (method_name_ != other.method_name_) {
    return false;
  }
  if (rpc_request_length_ != other.rpc_request_length_) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* RpcRequestHeader::Serialize() const {
  return RpcRequestHeader_reflection_->Serialize(this);
}

// DeSerialize()
void RpcRequestHeader::DeSerialize(const char* buf, unsigned int size) {
  RpcRequestHeader_reflection_->DeSerialize(this, buf, size);
}

// InitAsDefaultInstance()
void RpcRequestHeader::InitAsDefaultInstance() {
}

// swapper
void RpcRequestHeader::Swap(RpcRequestHeader* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  std::string service_name_tmp__ = std::move(other->mutable_service_name());
  other->mutable_service_name() = std::move(service_name_);
  service_name_ = std::move(service_name_tmp__);

  std::string method_name_tmp__ = std::move(other->mutable_method_name());
  other->mutable_method_name() = std::move(method_name_);
  method_name_ = std::move(method_name_tmp__);

  unsigned int rpc_request_length_tmp__ = other->rpc_request_length();
  other->set_rpc_request_length(rpc_request_length_);
  set_rpc_request_length(rpc_request_length_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const RpcRequestHeader& RpcRequestHeader::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
  }
  return *default_instance_;
}

RpcRequestHeader* RpcRequestHeader::default_instance_ = NULL;

// destructor
RpcRequestHeader::~RpcRequestHeader() {
}

// "service_name" = 1
bool RpcRequestHeader::has_service_name() const {
  return (has_bits_[0] & 0x2) != 0;
}

const std::string& RpcRequestHeader::service_name() const {
  return service_name_;
}

void RpcRequestHeader::set_service_name(const std::string& service_name) {
  service_name_ = service_name;
  has_bits_[0] |= 0x2;
}

void RpcRequestHeader::set_service_name(const char* service_name) {
  service_name_ = std::string(service_name);
  has_bits_[0] |= 0x2;
}

void RpcRequestHeader::set_service_name(const char* service_name, int size) {
  service_name_ = std::string(service_name, size);
  has_bits_[0] |= 0x2;
}

std::string RpcRequestHeader::mutable_service_name() {
  return service_name_;
}

void RpcRequestHeader::clear_service_name() {
  service_name_ = "";
  has_bits_[0] &= (~0x2);
}

// "method_name" = 2
bool RpcRequestHeader::has_method_name() const {
  return (has_bits_[0] & 0x4) != 0;
}

const std::string& RpcRequestHeader::method_name() const {
  return method_name_;
}

void RpcRequestHeader::set_method_name(const std::string& method_name) {
  method_name_ = method_name;
  has_bits_[0] |= 0x4;
}

void RpcRequestHeader::set_method_name(const char* method_name) {
  method_name_ = std::string(method_name);
  has_bits_[0] |= 0x4;
}

void RpcRequestHeader::set_method_name(const char* method_name, int size) {
  method_name_ = std::string(method_name, size);
  has_bits_[0] |= 0x4;
}

std::string RpcRequestHeader::mutable_method_name() {
  return method_name_;
}

void RpcRequestHeader::clear_method_name() {
  method_name_ = "";
  has_bits_[0] &= (~0x4);
}

// "rpc_request_length" = 3
bool RpcRequestHeader::has_rpc_request_length() const {
  return (has_bits_[0] & 0x8) != 0;
}

unsigned int RpcRequestHeader::rpc_request_length() const {
  return rpc_request_length_;
}

void RpcRequestHeader::set_rpc_request_length(unsigned int rpc_request_length) {
  rpc_request_length_ = rpc_request_length;
  has_bits_[0] |= 0x8;
}

void RpcRequestHeader::clear_rpc_request_length() {
  rpc_request_length_ = 0;
  has_bits_[0] &= (~0x8);
}

// ******************** RpcResponseHeader ******************** //
// constructor
RpcResponseHeader::RpcResponseHeader() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
RpcResponseHeader::RpcResponseHeader(const RpcResponseHeader& other) {
  CopyFrom(other);
}

// move constructor
RpcResponseHeader::RpcResponseHeader(RpcResponseHeader&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
RpcResponseHeader& RpcResponseHeader::operator=(const RpcResponseHeader& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
RpcResponseHeader& RpcResponseHeader::operator=(RpcResponseHeader&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* RpcResponseHeader::New() const {
  return reinterpret_cast<::proto::Message*>(new RpcResponseHeader());
}

// CopyFrom()
void RpcResponseHeader::CopyFrom(const RpcResponseHeader& other) {
  rpc_response_length_ = other.rpc_response_length();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void RpcResponseHeader::MoveFrom(RpcResponseHeader&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  rpc_response_length_ = other.rpc_response_length();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool RpcResponseHeader::Equals(const RpcResponseHeader& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (rpc_response_length_ != other.rpc_response_length_) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* RpcResponseHeader::Serialize() const {
  return RpcResponseHeader_reflection_->Serialize(this);
}

// DeSerialize()
void RpcResponseHeader::DeSerialize(const char* buf, unsigned int size) {
  RpcResponseHeader_reflection_->DeSerialize(this, buf, size);
}

// InitAsDefaultInstance()
void RpcResponseHeader::InitAsDefaultInstance() {
}

// swapper
void RpcResponseHeader::Swap(RpcResponseHeader* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  unsigned int rpc_response_length_tmp__ = other->rpc_response_length();
  other->set_rpc_response_length(rpc_response_length_);
  set_rpc_response_length(rpc_response_length_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const RpcResponseHeader& RpcResponseHeader::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_home_hy_Desktop_Snoopy_ProtoBuf_src_RPC_RpcSession();
  }
  return *default_instance_;
}

RpcResponseHeader* RpcResponseHeader::default_instance_ = NULL;

// destructor
RpcResponseHeader::~RpcResponseHeader() {
}

// "rpc_response_length" = 1
bool RpcResponseHeader::has_rpc_response_length() const {
  return (has_bits_[0] & 0x2) != 0;
}

unsigned int RpcResponseHeader::rpc_response_length() const {
  return rpc_response_length_;
}

void RpcResponseHeader::set_rpc_response_length(unsigned int rpc_response_length) {
  rpc_response_length_ = rpc_response_length;
  has_bits_[0] |= 0x2;
}

void RpcResponseHeader::clear_rpc_response_length() {
  rpc_response_length_ = 0;
  has_bits_[0] &= (~0x2);
}

}  // namespace RPC

