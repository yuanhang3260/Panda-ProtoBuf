#ifndef PROTO_DESCRIPTORS_BUILDER_
#define PROTO_DESCRIPTORS_BUILDER_

#include <string>
#include <memory>

#include "Base/MacroUtils.h"
#include "Proto/Descriptor.h"

namespace proto {

namespace ProtoParser {
class Parser;
class EnumType;
class Message;
class MessageField;
}

class DescriptorsBuilder {
 public:
  DescriptorsBuilder() = default;
  DescriptorsBuilder(const std::string& proto_file);

  ProtoFileDescriptor* BuildDescriptors();

 private:
  std::shared_ptr<EnumDescriptor>
  BuildEnumDescriptor(const ProtoParser::EnumType* enum_type,
                      const ProtoFileDescriptor* file,
                      bool nested);

  std::shared_ptr<FieldDescriptor> BuildFieldDescriptor(
                      const ProtoParser::MessageField* field,
                      int parse_index,
                      const ProtoFileDescriptor* file,
                      const MessageDescriptor* container_message);

  std::string proto_file_;

  FORBID_COPY_AND_ASSIGN(DescriptorsBuilder);
};

}  // namespace proto

#endif  // PROTO_DESCRIPTORS_BUILDER_
