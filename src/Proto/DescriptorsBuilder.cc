#include "Proto/DescriptorsBuilder.h"

#include <map>
#include <vector>
#include <set>
#include <memory>

#include "Base/Log.h"
#include "Base/BaseTypes.h"
#include "Compiler/EnumType.h"
#include "Compiler/Message.h"
#include "Compiler/MessageField.h"
#include "Compiler/ProtoParser.h"
#include "Compiler/ServiceType.h"
#include "Compiler/Type.h"

namespace proto {

using ProtoParser::Parser;

DescriptorsBuilder::DescriptorsBuilder(const std::string& proto_file) :
    proto_file_(proto_file) {
}

std::shared_ptr<ProtoFileDescriptor> DescriptorsBuilder::BuildDescriptors() {
  std::shared_ptr<ProtoFileDescriptor> file_dscpt;
  if (proto_file_.empty()) {
    LogERROR("No proto file path specified");
    return file_dscpt;
  }

  parser_.reset(new Parser(ProtoParser::CPP, proto_file_));
  if (!parser_->ParseProto()) {
    LogERROR("Failed to parse proto file %s", proto_file_.c_str());
    return file_dscpt;
  }

  file_dscpt.reset(new ProtoFileDescriptor(proto_file_));

  // Build file-level enum descriptors.
  for (auto& enum_type: parser_->enums_map_) {
    auto enum_dscpt = BuildEnumDescriptor(enum_type.second.get(),
                                          file_dscpt.get(), false);
    file_dscpt->impl_->AddEnumDescriptor(enum_dscpt);
  }

  // Build message descriptors.
  for (auto& message_type: parser_->messages_list_) {
    MessageDescriptor* msg_dscpt = new MessageDescriptor(
        file_dscpt.get(), message_type->name(), message_type->package()
    );
    file_dscpt->impl_->messages_map_.emplace(
        message_type->FullNameWithPackagePrefix(),
        std::shared_ptr<MessageDescriptor>(msg_dscpt)
    );

    // Nested enum types.
    for (auto& nested_enum: message_type->enums_map()) {
      auto enum_dscpt = BuildEnumDescriptor(nested_enum.second.get(),
                                            file_dscpt.get(), true);
      // Add to file-level and message-level enum maps.
      file_dscpt->impl_->AddEnumDescriptor(enum_dscpt);
      msg_dscpt->impl_->AddNestedEnumDescriptor(enum_dscpt);
    }
  }

  // TODO(hangyuan): Build RPC service descriptors.

  // Add field descriptors to message descriptors.
  for (auto& message_type: parser_->messages_list_) {
    auto it = file_dscpt->impl_->messages_map_.find(
                        message_type->FullNameWithPackagePrefix());
    if (it == file_dscpt->impl_->messages_map_.end()) {
      LogFATAL("Can't find generated message descriptor - Where is it ??");
    }
    auto msg_dscpt = it->second.get();
    for (auto& field: message_type->fields_list()) {
      auto field_dscpt = BuildFieldDescriptor(field.get(),
                                              file_dscpt.get(), msg_dscpt);
      std::string field_name = field_dscpt->name();
      msg_dscpt->impl_->fields_map_.emplace(field_name, field_dscpt);
      msg_dscpt->impl_->fields_list_.emplace_back(field_dscpt);
      msg_dscpt->impl_->tag_fields_map_.emplace(field_dscpt->tag(),field_dscpt);
    }
  }

  return file_dscpt;
}

std::shared_ptr<EnumDescriptor> DescriptorsBuilder::BuildEnumDescriptor(
    const ProtoParser::EnumType* enum_type,
    const ProtoFileDescriptor* file,
    bool nested) {
  auto enum_dscpt = new EnumDescriptor(file, enum_type->name(),
                                       enum_type->package(), nested);
  int i = 0;
  for (const auto& enum_value: enum_type->enums()) {
    enum_dscpt->impl_->enums_map_.emplace(i++, enum_value);
    enum_dscpt->impl_->enums_str_set_.insert(enum_value);
  }

  return std::shared_ptr<EnumDescriptor>(enum_dscpt);
}

std::shared_ptr<FieldDescriptor> DescriptorsBuilder::BuildFieldDescriptor(
    const ProtoParser::MessageField* field,
    const ProtoFileDescriptor* file,
    const MessageDescriptor* container_message) {
  const TypeDescriptor* type_descriptor = nullptr;
  if (field->type() == MESSAGETYPE || field->type() == ENUMTYPE) {
    std::string type_name = field->type_class()->FullNameWithPackagePrefix();
    if ((type_descriptor = file->FindMessageTypeByName(type_name)) == nullptr &&
        (type_descriptor = file->FindEnumTypeByName(type_name)) == nullptr) {
      LogFATAL("Can't find message or enum descriptor for "
               "field %s in message %s",
               field->name().c_str(), container_message->full_name().c_str());
    }
  }

  // field offset waits to be set.
  auto field_dscpt = new FieldDescriptor(field->name(), field->modifier(),
                                         field->type(), field->tag(),
                                         field->default_value(),
                                         container_message,
                                         type_descriptor, -1);

  return std::shared_ptr<FieldDescriptor>(field_dscpt);
}

}  // namespace proto
