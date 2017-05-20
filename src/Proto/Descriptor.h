#ifndef PROTO_DESCRIPTOR_
#define PROTO_DESCRIPTOR_

#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>

#include "Base/BaseTypes.h"
#include "Base/MacroUtils.h"
#include "Proto/Common.h"

namespace proto {

/// Forward declarations.
class MessageDescriptor;
class EnumDescriptor;
class FieldDescriptor;
class ServiceDescriptor;
class DescriptorsBuilder;
class MessageReflection;

/// ProtoFileDescriptor
class ProtoFileDescriptor {
 public:
  ProtoFileDescriptor(const std::string& proto_content);
  ~ProtoFileDescriptor();

  const std::string& proto_content() const;
  int num_messages() const;
  int num_enums() const;
  int num_nested_enums() const;
  int num_services() const;

  const EnumDescriptor*
  FindEnumTypeByName(const string& enum_name) const;
  
  const MessageDescriptor*
  FindMessageTypeByName(const string& message_name) const;
  
  const ServiceDescriptor*
  FindSeriveTypeByName(const string& service_name) const;

 private:
  class ProtoFileDescriptorImpl;
  std::unique_ptr<ProtoFileDescriptorImpl> impl_;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
};


/// TypeDescriptor is the base class of Message, Enum and Service Descriptors.
class TypeDescriptor {
 public:
  TypeDescriptor(const ProtoFileDescriptor* file,
                 const std::string& name, const std::string& package);
  virtual ~TypeDescriptor();

  virtual void Print() {}
  virtual FieldType type() = 0;

  DEFINE_GETTER(name, std::string);
  DEFINE_GETTER(package, std::string);
  DEFINE_CONST_PTR_GETTER(file, ProtoFileDescriptor);
  std::string full_name() const;

 protected:
  const ProtoFileDescriptor* file_;
  std::string name_;
  std::string package_;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
};


/// MessageDescriptor
class MessageDescriptor: public TypeDescriptor {
 public:
  MessageDescriptor(const ProtoFileDescriptor* file,
                    const std::string& name, const std::string& package);
  virtual ~MessageDescriptor();

  FieldType type() override { return MESSAGETYPE; }
  int num_fields() const;
  int num_nested_enums() const;

  // Find field descriptor.
  const FieldDescriptor* FindFieldByTag(uint32 tag) const;
  const FieldDescriptor* FindFieldByName(const std::string& field_name) const;

  // Find nested defined enum types in the message.
  const EnumDescriptor* FindNestedEnumTypeByName(const std::string& name) const;

 private:
  class MessageDescriptorImpl;
  std::unique_ptr<MessageDescriptorImpl> impl_;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
};

using Descriptor = MessageDescriptor;


/// EnumDescriptor
class EnumDescriptor: public TypeDescriptor {
 public:
  EnumDescriptor(const ProtoFileDescriptor* file,
                 const std::string& name, const std::string& package,
                 bool nested);
  virtual ~EnumDescriptor();

  FieldType type() override { return ENUMTYPE; }

  int NumberEnums() const;
  bool ContainsEnum(uint32 enum_value) const;
  bool ContainsEnum(const std::string& enum_value_str) const;

  bool is_nested() const;

  std::string EnumValueAsString(int value) const;

 private:
  class EnumDescriptorImpl;
  std::unique_ptr<EnumDescriptorImpl> impl_;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
};

/// ServiceDescriptor
class ServiceDescriptor: public TypeDescriptor {
 public:
  ServiceDescriptor(const ProtoFileDescriptor* file,
                    const std::string& name, const std::string& package);

  virtual ~ServiceDescriptor();

  FieldType type() override { return SERVICETYPE; }

 private:
  class ServiceDescriptorImpl;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
};

/// MessageField descriptor.
class FieldDescriptor {
 public:
  FieldDescriptor(std::string name, FieldLabel label, FieldType type,
                  int tag, std::string default_value,
                  const MessageDescriptor* container_message,
                  const TypeDescriptor* type_descriptor,
                  int parse_index);
  virtual ~FieldDescriptor() {}

  // full_name() is really odd. For example, if type_descriptor is "Foo.Bar"
  // and field name is "hostname", fullname will be "Foo.Bar.hostname". It's a
  // combination of message package path plus field name, which looks a bit
  // unreasonable. But it's useful because it can uniquely identify a message
  // field within global scope.
  DEFINE_GETTER(name, std::string);
  DEFINE_GETTER(label, FieldLabel);
  DEFINE_GETTER(type, FieldType);
  DEFINE_GETTER(tag, uint32);
  DEFINE_GETTER(default_value, std::string);
  std::string full_name() const;

  DEFINE_CONST_PTR_GETTER(container_message, MessageDescriptor);
  DEFINE_CONST_PTR_GETTER(type_descriptor, TypeDescriptor);

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

  //int field_offset_ = -1;
  int parse_index_ = -1;

  friend class DescriptorsBuilder;
  friend class MessageReflection;
};

}

#endif  // PROTO_DESCRIPTOR_
