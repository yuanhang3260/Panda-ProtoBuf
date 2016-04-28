#include "Proto/Descriptor.h"
#include "Proto/Descriptors_internal.h"

#include "Base/MarcoUtils.h"

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
  return impl_->enums_map_.size()
}

int ProtoFileDescriptor::num_nested_enums() const {
  return impl_->enums_map_.size()
}

int ProtoFileDescriptor::num_services() const {
  return impl_->services_map_.size()
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
    TypeDescriptor(file, name, package);
    impl_(new MessageDescriptorImpl()) {
}

const FieldDescriptor* MessageDescriptor::FindFieldByTag(
    unsigned int tag) const {
  auto it = impl_->tag_fields_map_.find(tag);
  if (it != impl_->tag_fields_map_.end()) {
    return it.second.get();
  }
  return nullptr;
}

const FieldDescriptor* FindFieldByName(std::string field_name) const {
  auto it = impl_->fields_map_.find(field_name);
  if (it != impl_->fields_map_.end()) {
    return it.second.get();
  }
  return nullptr;
}

const EnumDescriptor* FindNestedEnumTypeByName(const std::string& name) const {
  auto it = impl_->enums_map_.find(name);
  if (it != impl_->enums_map_.end()) {
    return it.second.get();
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

bool ContainsEnum(const std::string& enum_value) const {
  return impl_->enums_str_set_.find(enum_value) != impl_->enums_str_set_.end();
}

bool EnumDescriptor::is_nested() const {
  return impl_->is_nested_;
}

std::string EnumDescriptor::EnumValueAsString(int value) const {
  if (impl_->enums_map_.find(value) != impl_->enums_map_.end()) {
    return impl_->enums_map_.at(valaue);
  }
  return "";
}

}  // namespace proto