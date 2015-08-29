#include "WireFormat.h"
#include "MessageFactory.h"
#include "MessageReflection.h"


namespace proto {

MessageReflection::MessageReflection(
    std::shared_ptr<::proto::ProtoParser::Message> message_descirptor,
    Message* defatult_instance,
    int has_bits_offset) :
  message_descirptor_(message_descirptor),
  defatult_instance_(defatult_instance),
  has_bits_offset_(has_bits_offset) {
}

const ::proto::ProtoParser::Message*
MessageReflection::descriptor() {
  return message_descirptor_.get();
}

const Message* MessageReflection::defatult_instance() {
  return defatult_instance_;
}

SerializedMessage* MessageReflection::Serialize(const Message* message) const {
  SerializedMessage* sdmsg = new SerializedMessage();
  // Begin serializing a message
  for (const auto& field: message_descirptor_->fields_list()) {
    if (!HasField(message, field->tag())) {
      continue;
    }
    // Message Type
    if (field->IsMessageType()) {
      const MessageReflection* nested_msg_reflection =
          MessageFactory::GetMessageReflection(
              message_descirptor_->FullNameWithPackagePrefix(ProtoParser::CPP));
      if (!nested_msg_reflection) {
        throw std::runtime_error(
            "No cpp generated class type " +
            message_descirptor_->FullNameWithPackagePrefix(ProtoParser::CPP) +
            " exists.");
      }
      SerializedMessage* nested_sdmsg = nested_msg_reflection->Serialize(
        reinterpret_cast<const Message*>(
          (reinterpret_cast<const char*>(message) + field->field_offset())
        )
      );
      sdmsg->AddField(std::shared_ptr<SerializedObjectInterface>(nested_sdmsg));
    }
    // Primitive Type
    else {
            
    }
  }
  return sdmsg;
}

bool MessageReflection::HasField(const Message* message, int tag) const {
  const char* has_bits =
    reinterpret_cast<const char*>(message) + has_bits_offset_;
  return (has_bits[tag / 8] & (0x1 << (tag % 8))) != 0;
}

}  // namespace proto