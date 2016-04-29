#include "Proto/Descriptor.h"
#include "Proto/Descriptors_internal.h"

#include "Base/MacroUtils.h"

namespace proto {

/// ProtoFileDescriptor
ProtoFileDescriptor::ProtoFileDescriptor(const std::string& path) :
    impl_(new ProtoFileDescriptorImpl(path)) {
}

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
  return impl_->enums_map_.size();
}

int ProtoFileDescriptor::num_services() const {
  return impl_->services_map_.size();
}


/// TypeDescriptor
TypeDescriptor::TypeDescriptor(const ProtoFileDescriptor* file,
                               const std::string& name,
                               const std::string& package) :
    file_(file), name_(name), package_(package) {
}


DEFINE_GETTER(TypeDescriptor, name, std::string);
DEFINE_GETTER(TypeDescriptor, package, std::string);
DEFINE_CONST_PTR_GETTER(TypeDescriptor, file, ProtoFileDescriptor);

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

int EnumDescriptor::NumberEnums() const {
  return impl_->enums_map_.size();
}

bool EnumDescriptor::ContainsEnum(const std::string& enum_value) const {
  return impl_->enums_str_set_.find(enum_value) != impl_->enums_str_set_.end();
}

bool EnumDescriptor::is_nested() const {
  return impl_->is_nested_;
}

std::string EnumDescriptor::EnumValueAsString(int value) const {
  if (impl_->enums_map_.find(value) != impl_->enums_map_.end()) {
    return impl_->enums_map_.at(value);
  }
  return "";
}


/// FieldDescriptor
FieldDescriptor::FieldDescriptor(std::string name,
                                 FieldLabel label, FieldType type,
                                 int tag, std::string default_value,
                                 const MessageDescriptor* container_message,
                                 const TypeDescriptor* type_descriptor,
                                 int field_offset) :
    name_(name), label_(label), type_(type),
    tag_(tag), default_value_(default_value),
    container_message_(container_message),
    type_descriptor_(type_descriptor),
    field_offset_(field_offset) {
}

DEFINE_GETTER(FieldDescriptor, name, std::string);
DEFINE_GETTER(FieldDescriptor, label, FieldLabel);
DEFINE_GETTER(FieldDescriptor, type, FieldType);
DEFINE_GETTER(FieldDescriptor, tag, uint32);
DEFINE_GETTER(FieldDescriptor, default_value, std::string);
DEFINE_CONST_PTR_GETTER(FieldDescriptor, container_message, MessageDescriptor);
DEFINE_CONST_PTR_GETTER(FieldDescriptor, type_descriptor, TypeDescriptor);

std::string FieldDescriptor::full_name() const {
  return container_message_->full_name() + "." + name_;
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
