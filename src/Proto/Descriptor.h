#ifndef PROTO_DESCRIPTOR_
#define PROTO_DESCRIPTOR_

#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>

#include "Base/BaseTypes.h"
#include "Proto/Common.h"

namespace proto {

/// Forward declarations.
class MessageDescriptor;
class EnumDescriptor;
class FieldDescriptor;
class ServiceDescriptor;
class FieldDescriptor;


/// ProtoFileDescriptor
class ProtoFileDescriptorImpl;
class ProtoFileDescriptor {
 public:
  ProtoFileDescriptor(const std::string& path);
  std::string path() const;
  int num_messages() const;
  int num_enums() const;
  int num_nested_enums() const;
  int num_services() const;

 private:
  std::unique_ptr<ProtoFileDescriptorImpl> impl_;
};


/// TypeDescriptor is the base class of Message, Enum and Service Descriptors.
class TypeDescriptor {
 public:
  TypeDescriptor(const ProtoFileDescriptor* file,
                 const std::string& name, const std::string& package);
  virtual ~TypeDescriptor() {}

  virtual void Print() {}
  virtual FieldType type() = 0;
  const ProtoFileDescriptor* file() const;

  std::string name() const;
  std::string package() const;
  std::string full_name() const;

 protected:
  const ProtoFileDescriptor* file_;
  std::string name_;
  std::string package_;
};


/// MessageDescriptor
class MessageDescriptorImpl;
class MessageDescriptor: public TypeDescriptor {
 public:
  MessageDescriptor(const ProtoFileDescriptor* file,
                    const std::string& name, const std::string& package);
  virtual ~MessageDescriptor();

  FieldType type() override { return MESSAGETYPE; }

  // Find field descriptor.
  const FieldDescriptor* FindFieldByTag(uint32 tag) const;
  const FieldDescriptor* FindFieldByName(const std::string& field_name) const;

  // Find nested defined enum types in the message.
  const EnumDescriptor* FindNestedEnumTypeByName(const std::string& name) const;

 private:
  std::unique_ptr<MessageDescriptorImpl> impl_;
};

using Descriptor = MessageDescriptor;


/// EnumDescriptor
class EnumDescriptorImpl;
class EnumDescriptor: public TypeDescriptor {
 public:
  EnumDescriptor(const ProtoFileDescriptor* file,
                 const std::string& name, const std::string& package,
                 bool nested);

  FieldType type() override { return ENUMTYPE; }

  int NumberEnums() const;
  bool ContainsEnum(const std::string& enum_value) const;

  bool is_nested() const;

  std::string EnumValueAsString(int value) const;

 protected:
  std::unique_ptr<EnumDescriptorImpl> impl_;
};

/// ServiceDescriptor
class ServiceDescriptorImpl;
class ServiceDescriptor: public TypeDescriptor {
 public:
  ServiceDescriptor(const ProtoFileDescriptor* file,
                    const std::string& name, const std::string& package);

  FieldType type() override { return SERVICETYPE; }
};

/// MessageField descriptor.
class FieldDescriptor {
 public:
  FieldDescriptor(std::string name, FieldLabel label, FieldType type,
                  int tag, std::string default_value,
                  const MessageDescriptor* container_message,
                  const TypeDescriptor* type_descriptor,
                  int field_offset);
  virtual ~FieldDescriptor();

  // full_name() is really odd. For example, if container_message is "Foo.Bar"
  // and field name is "hostname", fullname will be "Foo.Bar.hostname". It's a
  // combination of message package path plus field name, which looks a bit
  // unreasonable. But it's useful because it can uniquely identify a message
  // field within global scope.
  std::string full_name() const;
  std::string name() const;
  FieldLabel label() const;
  FieldType type() const;
  uint32 tag() const;
  std::string default_value() const;
  const MessageDescriptor* container_message() const;
  const TypeDescriptor* type_descriptor() const;

  int field_offset() const;
  bool has_default_value() const;

  bool IsPrimitiveType() const;
  bool IsMessageType() const;
  bool IsSingularType() const;
  bool IsRepeatedType() const;
  bool IsSingularNumericType() const;
  bool IsSingularStringType() const;
  bool IsSingularMessageType() const;
  bool IsRepeatedNumericType() const;
  bool IsRepeatedStringType() const;
  bool IsRepeatedMessageType() const;

 private:
  std::string name_;
  FieldLabel label_;
  FieldType type_;
  uint32 tag_;
  std::string default_value_;

  // Parent message of this field.
  const MessageDescriptor* container_message_;
  // Type descriptor for enum or message type of this field.
  // For primitive fields (string included), type_descriptor is nullptr.
  const TypeDescriptor* type_descriptor_;

  bool has_default_value_ = false;
  int field_offset_ = -1;
};

}

#endif  /* PROTO_DESCRIPTOR_ */
