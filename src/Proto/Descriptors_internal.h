#ifndef PROTO_DESCRIPTOR_INTERNAL_
#define PROTO_DESCRIPTOR_INTERNAL_

#include <map>
#include <vector>
#include <set>
#include <string>
#include <memory>

namespace proto {

// Forward declarations.
class ProtoFileDescriptor;
class MessageDescriptor;
class EnumDescriptor;
class FieldDescriptor;
class ServiceDescriptor;
class FieldDescriptor;

/// ProtoFileDescriptorImpl

class ProtoFileDescriptorImpl {
 public:
  ProtoFileDescriptorImpl(std::string path) : path_(path) {}

  // Add descriptors (take ownership of argument)
  void AddMessageDescriptor(MessageDescriptor* descriptor);
  void AddEnumDescriptor(EnumDescriptor* descriptor);
  void AddServiceDescriptor(ServiceDescriptor* descriptor);

 private:
  std::string path_;

  std::map<std::string, std::shared_ptr<MessageDescriptor>> messages_map_;
  std::map<std::string, std::shared_ptr<EnumDescriptor>> enums_map_;
  std::map<std::string, std::shared_ptr<ServiceDescriptor>> services_map_;

  friend class ProtoFileDescriptor;
  FORBID_COPY_AND_ASSIGN(ProtoFileDescriptorImpl);
};


/// MessageDescriptorImpl
class MessageDescriptorImpl {
 public:
  MessageDescriptorImpl() = default;

 private:
  std::map<std::string, std::shared_ptr<FieldDescriptor>> fields_map_;
  std::map<unsigned int, std::shared_ptr<FieldDescriptor>> tag_fields_map_;
  std::map<std::string, std::shared_ptr<EnumDescriptor>> enums_map_;

  friend class MessageDescriptor;
  FORBID_COPY_AND_ASSIGN(MessageDescriptorImpl);
};


class EnumDescriptorImpl: public TypeDescriptor {
 public:
  EnumDescriptorImpl(bool nested);

 protected:
  bool is_nested_;
  std::map<int, std::string> enums_map_;
  std::set<std::string> enums_str_set_;

  friend class EnumDescriptor;
  FORBID_COPY_AND_ASSIGN(EnumDescriptorImpl);
};

}

#endif  /* PROTO_DESCRIPTOR_INTERNAL_ */
