#ifndef PROTO_DESCRIPTOR_
#define PROTO_DESCRIPTOR_

#include <string>

namespace proto {

// Forward declarations.
class MessageDescriptor;
class EnumDescriptor;
class FieldDescriptor;
class ServiceDescriptor;
class FieldDescriptor;


// ProtoFileDescriptor
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


// TypeDescriptor is the base class of Message, Enum and Service Descriptors.
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
  std::string name_;
  std::string package_;
  const ProtoFileDescriptor* file_;
};


// MessageDescriptor
class MessageDescriptorImpl;
class MessageDescriptor: public TypeDescriptor {
 public:
  MessageDescriptor(const ProtoFileDescriptor* file,
                    const std::string& name, const std::string& package);
  virtual ~MessageDescriptor();

  FieldType type() override { return MESSAGETYPE; }

  // Find field descriptor.
  const FieldDescriptor* FindFieldByTag(unsigned int tag) const;
  const FieldDescriptor* FindFieldByName(const std::string& field_name) const;

  // Find nested defined enum types in the message.
  const EnumDescriptor* FindNestedEnumTypeByName(const std::string& name) const;

 private:
  std::unique_ptr<MessageDescriptorImpl> impl_;
};

using Descriptor = MessageDescriptor;


// EnumDescriptor
class EnumDescriptorImpl;
class EnumDescriptor: public TypeDescriptor {
 public:
  EnumDescriptor(const ProtoFileDescriptor* file,
                 const std::string& name, const std::string& package,
                 bool nested);

  int NumberEnums() const;
  bool ContainsEnum(const std::string& enum_value) const;

  bool is_nested() const;

  std::string EnumValueAsString(int value) const;

 protected:
  std::map<int, std::string> enums_map_;
  std::set<std::string> enums_set_;
};


// ServiceDescriptor
class ServiceDescriptorImpl;
class ServiceDescriptor: public TypeDescriptor {
 public:
  ServiceDescriptor(const ProtoFileDescriptor* file,
                    const std::string& name, const std::string& package);
};


// MessageField descriptor.
class FieldDescriptor {
 public:
  FieldDescriptor(std::string name, int tag, std::string default_value,
                  FieldLabel label, FieldType type,
                  const MessageDescriptor* container_message,
                  const TypeDescriptor* descriptor);
  virtual ~FieldDescriptor();

  std::string name() const { return name_; }
  std::string full_name() const;
  std::string type_name() const { return type_name_; }

  FieldLabel label() const { return modifier_; }
  FieldType type() const { return type_; }

  const PbType* type_class() const { return type_class_; }

  unsigned int tag() const { return tag_; }
  std::string default_value() const { return default_value_; }

  int field_offset() const { return field_offset_; }
  bool has_default_value() const { return has_user_default_value_; }

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
  void set_field_offset(const int offset) { field_offset_ = offset; }
  void set_type_name(std::string type_name) { type_name_ = type_name; }

  std::string name_;
  FieldLabel label_;
  FieldType type_;
  unsigned int tag_;

  // Type descriptor for enum or message type of this field.
  // If it's a primitive field (string included), type_descriptor is nullptr.
  const TypeDescriptor* type_descriptor;

  std::string default_value_;
  bool has_default_value_ = false;
  std::string parent_field_path_
  int field_offset_ = -1;
};

}

#endif  /* PROTO_DESCRIPTOR_ */
