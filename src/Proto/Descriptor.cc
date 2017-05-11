#include "Proto/Descriptor.h"
#include "Proto/Descriptors_internal.h"

#include "Base/Log.h"

namespace proto {

/// ProtoFileDescriptor
ProtoFileDescriptor::ProtoFileDescriptor(const std::string& path) :
    impl_(new ProtoFileDescriptorImpl(path)) {
}

ProtoFileDescriptor::~ProtoFileDescriptor() {}

std::string ProtoFileDescriptor::path() const {
  return impl_->path_;
}

int ProtoFileDescriptor::num_messages() const {
  return impl_->messages_map_.size();
}

int ProtoFileDescriptor::num_enums() const {
  return impl_->enums_map_.size();
}

int ProtoFileDescriptor::num_nested_enums() const {
  return impl_->num_nested_enums_;
}

int ProtoFileDescriptor::num_services() const {
  return impl_->services_map_.size();
}

const EnumDescriptor*
ProtoFileDescriptor::FindEnumTypeByName(const string& enum_name) const {
  auto it = impl_->enums_map_.find(enum_name);
  if (it != impl_->enums_map_.end()) {
    return it->second.get();
  }
  return nullptr;
}

const MessageDescriptor*
ProtoFileDescriptor::FindMessageTypeByName(const string& message_name) const {
  auto it = impl_->messages_map_.find(message_name);
  if (it != impl_->messages_map_.end()) {
    return it->second.get();
  }
  return nullptr;
}

const ServiceDescriptor*
ProtoFileDescriptor::FindSeriveTypeByName(const string& service_name) const {
  auto it = impl_->services_map_.find(service_name);
  if (it != impl_->services_map_.end()) {
    return it->second.get();
  }
  return nullptr;
}

/// TypeDescriptor
TypeDescriptor::TypeDescriptor(const ProtoFileDescriptor* file,
                               const std::string& name,
                               const std::string& package) :
    file_(file), name_(name), package_(package) {
}

TypeDescriptor::~TypeDescriptor() {}

std::string TypeDescriptor::full_name() const {
  return package_ + "." + name_;
}

/// MessageDescriptor.
MessageDescriptor::MessageDescriptor(const ProtoFileDescriptor* file,
                                     const std::string& name,
                                     const std::string& package) :
    TypeDescriptor(file, name, package),
    impl_(new MessageDescriptorImpl()) {
}

MessageDescriptor::~MessageDescriptor() {}

int MessageDescriptor::num_fields() const {
  return impl_->fields_map_.size();
}

int MessageDescriptor::num_nested_enums() const {
  return impl_->enums_map_.size();
}

const FieldDescriptor* MessageDescriptor::FindFieldByTag(
    unsigned int tag) const {
  auto it = impl_->tag_fields_map_.find(tag);
  if (it != impl_->tag_fields_map_.end()) {
    return it->second.get();
  }
  return nullptr;
}

const FieldDescriptor*
MessageDescriptor::FindFieldByName(const std::string& field_name) const {
  auto it = impl_->fields_map_.find(field_name);
  if (it != impl_->fields_map_.end()) {
    return it->second.get();
  }
  return nullptr;
}

const EnumDescriptor*
MessageDescriptor::FindNestedEnumTypeByName(const std::string& name) const {
  auto it = impl_->enums_map_.find(name);
  if (it != impl_->enums_map_.end()) {
    return it->second.get();
  }
  return nullptr;
}


/// EnumDescriptor
EnumDescriptor::EnumDescriptor(const ProtoFileDescriptor* file,
                               const std::string& name,
                               const std::string& package,
                               bool nested) :
    TypeDescriptor(file, name, package),
    impl_(new EnumDescriptorImpl(nested)) {
}

EnumDescriptor::~EnumDescriptor() {}

int EnumDescriptor::NumberEnums() const {
  return impl_->enums_map_.size();
}

bool EnumDescriptor::ContainsEnum(uint32 enum_value) const {
  return impl_->enums_map_.find(enum_value) != impl_->enums_map_.end();
}

bool EnumDescriptor::ContainsEnum(const std::string& enum_value) const {
  return impl_->enums_str_set_.find(enum_value) != impl_->enums_str_set_.end();
}

bool EnumDescriptor::is_nested() const {
  return impl_->is_nested_;
}

std::string EnumDescriptor::EnumValueAsString(int value) const {
  auto it = impl_->enums_map_.find(value);
  if (it != impl_->enums_map_.end()) {
    return it->second;
  }
  return "";
}


/// ServiceDescriptor
ServiceDescriptor::~ServiceDescriptor() {}

/// FieldDescriptor
FieldDescriptor::FieldDescriptor(std::string name,
                                 FieldLabel label, FieldType type,
                                 int tag, std::string default_value,
                                 const MessageDescriptor* container_message,
                                 const TypeDescriptor* type_descriptor,
                                 int parse_index) :
    name_(name), label_(label), type_(type),
    tag_(tag), default_value_(default_value),
    container_message_(container_message),
    type_descriptor_(type_descriptor),
    parse_index_(parse_index) {
}

std::string FieldDescriptor::full_name() const {
  return type_descriptor_->package() + "." + name_;
}

bool FieldDescriptor::has_default_value() const {
  return !default_value_.empty();
}

bool FieldDescriptor::IsPrimitiveType() const {
  return type_ != STRING && type_ != MESSAGETYPE;
}

bool FieldDescriptor::IsMessageType() const {
  return type_ == MESSAGETYPE;
}

bool FieldDescriptor::IsRepeatedType() const {
  return label_ == REPEATED;
}

bool FieldDescriptor::IsSingularType() const {
  return label_ != REPEATED;
}

bool FieldDescriptor::IsSingularNumericType() const {
  return label_ != REPEATED &&
         type_ != MESSAGETYPE && type_ != STRING;
}

bool FieldDescriptor::IsSingularStringType() const {
  return label_ != REPEATED &&
         type_ == STRING;
}

bool FieldDescriptor::IsSingularMessageType() const {
  return label_ != REPEATED &&
         type_ == MESSAGETYPE;
}

bool FieldDescriptor::IsRepeatedNumericType() const {
  return label_ == REPEATED &&
         type_ != MESSAGETYPE && type_ != STRING;
}

bool FieldDescriptor::IsRepeatedStringType() const {
  return label_ == REPEATED &&
         type_ == STRING;
}

bool FieldDescriptor::IsRepeatedMessageType() const {
  return label_ == REPEATED &&
         type_ == MESSAGETYPE;
}


}  // namespace proto
