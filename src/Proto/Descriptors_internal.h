#ifndef PROTO_DESCRIPTOR_INTERNAL_
#define PROTO_DESCRIPTOR_INTERNAL_

#include <map>
#include <vector>
#include <set>
#include <string>
#include <memory>

#include "Base/BaseTypes.h"
#include "Base/MacroUtils.h"
#include "Proto/Descriptor.h"

namespace proto {

// Forward declarations.
class DescriptorsBuilder;

class ProtoFileDescriptor::ProtoFileDescriptorImpl {
 public:
  ProtoFileDescriptorImpl(std::string path) : path_(path) {}
  ~ProtoFileDescriptorImpl() {}

  // Add descriptors.
  void AddMessageDescriptor(std::shared_ptr<MessageDescriptor> descriptor);
  void AddEnumDescriptor(std::shared_ptr<EnumDescriptor> descriptor);
  void AddServiceDescriptor(std::shared_ptr<ServiceDescriptor> descriptor);

 private:
  std::string path_;
  int num_nested_enums_ = 0;

  std::map<std::string, std::shared_ptr<MessageDescriptor>> messages_map_;
  std::map<std::string, std::shared_ptr<EnumDescriptor>> enums_map_;
  std::map<std::string, std::shared_ptr<ServiceDescriptor>> services_map_;

  friend class ProtoFileDescriptor;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
  FORBID_COPY_AND_ASSIGN(ProtoFileDescriptorImpl);
};


/// MessageDescriptorImpl
class MessageDescriptor::MessageDescriptorImpl {
 public:
  MessageDescriptorImpl() = default;
  ~MessageDescriptorImpl() {}

  void AddNestedEnumDescriptor(std::shared_ptr<EnumDescriptor> descriptor);

 private:
  std::map<std::string, std::shared_ptr<FieldDescriptor>> fields_map_;
  std::vector<std::shared_ptr<FieldDescriptor>> fields_list_;
  std::map<uint32, std::shared_ptr<FieldDescriptor>> tag_fields_map_;
  std::map<std::string, std::shared_ptr<EnumDescriptor>> enums_map_;

  friend class MessageDescriptor;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
  FORBID_COPY_AND_ASSIGN(MessageDescriptorImpl);
};


class EnumDescriptor::EnumDescriptorImpl {
 public:
  EnumDescriptorImpl(bool nested);
  ~EnumDescriptorImpl() {}

 protected:
  bool is_nested_;
  std::map<int, std::string> enums_map_;
  std::set<std::string> enums_str_set_;

  friend class EnumDescriptor;
  friend class DescriptorsBuilder;
  friend class MessageReflection;
  FORBID_COPY_AND_ASSIGN(EnumDescriptorImpl);
};

}

#endif  // PROTO_DESCRIPTOR_INTERNAL_
