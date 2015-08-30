#include "WireFormat.h"
#include "RepeatedFields.h"
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
      if (!field->IsRepeatedType()) {
        sdmsg->AddField(CreateSerializedSingularMessage(message, field.get()));
      }
      else {
        sdmsg->AddField(CreateSerializedRepeatedMessage(message, field.get()));
      }
    }
    // Primitive Type and String type
    else {
      if (!field->IsRepeatedType()) {
        sdmsg->AddField(CreateSerializedSingularPrimitive(message, field.get()));
      }
      else {
        sdmsg->AddField(CreateSerializedRepeatedPrimitive(message, field.get()));
      }
    }
  }
  return sdmsg;
}

std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedSingularMessage(
    const Message* message,
    const ProtoParser::MessageField* field)  const {

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
  return std::shared_ptr<SerializedObjectInterface>(nested_sdmsg);
}

std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedRepeatedMessage(
    const Message* message,
    const ProtoParser::MessageField* field)  const {
  // Create a new SerializedMessage to store repeated messages.
  SerializedMessage* sdmsg = new SerializedMessage();
  // get reflection
  const MessageReflection* nested_msg_reflection =
      MessageFactory::GetMessageReflection(
          message_descirptor_->FullNameWithPackagePrefix(ProtoParser::CPP));
  if (!nested_msg_reflection) {
    throw std::runtime_error(
        "No cpp generated class type " +
        message_descirptor_->FullNameWithPackagePrefix(ProtoParser::CPP) +
        " exists.");
  }
  // Get RepeatedMessagePtr
  const char* field_addr =
      reinterpret_cast<const char*>(message) + field->field_offset();
  const auto repeated_field =
      reinterpret_cast<const RepeatedFieldBase*>(field_addr);
  // Serialize repeated messages one by one.
  for (uint32 i = 0; i < repeated_field->size(); i++) {
    const char* element_ptr = repeated_field->GetElementPtr(i);
    SerializedMessage* nested_sdmsg = nested_msg_reflection->Serialize(
      reinterpret_cast<const Message*>(element_ptr)
    );
    sdmsg->AddField(std::shared_ptr<SerializedObjectInterface>(nested_sdmsg));
  }
  return std::shared_ptr<SerializedObjectInterface>(sdmsg);
}

#define ENCODE_SINGULAR_PRITIMIVE(CPP_TYPE, WIRE_TYPENAME)                   \
  WireFormat::Encode##WIRE_TYPENAME(                                         \
    field->tag(),                                                            \
    *reinterpret_cast<const CPP_TYPE*>(msg_addr + field->field_offset()),    \
    sdprim->mutable_ostream());

std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedSingularPrimitive(
    const Message* message,
    const ProtoParser::MessageField* field)  const {

  SerializedPrimitive* sdprim = new SerializedPrimitive(field->type());
  const char* msg_addr = reinterpret_cast<const char*>(message);
  switch (field->type()) {
    case ProtoParser::UINT32:
      ENCODE_SINGULAR_PRITIMIVE(uint32, UInt32)
      break;
    case ProtoParser::UINT64:
      ENCODE_SINGULAR_PRITIMIVE(uint64, UInt64)
      break;
    case ProtoParser::INT32:
      ENCODE_SINGULAR_PRITIMIVE(int32, SInt32)
      break;
    case ProtoParser::INT64:
      ENCODE_SINGULAR_PRITIMIVE(int64, SInt64)
      break;
    case ProtoParser::BOOL:
      ENCODE_SINGULAR_PRITIMIVE(bool, Bool)
      break;
    case ProtoParser::DOUBLE:
      ENCODE_SINGULAR_PRITIMIVE(double, Double)
      break;
    case ProtoParser::STRING:
      ENCODE_SINGULAR_PRITIMIVE(std::string, String)
      break;
    case ProtoParser::ENUMTYPE:
      ENCODE_SINGULAR_PRITIMIVE(uint32, UInt32)
      break;
    default:
      throw std::runtime_error(
                "type " +
                ProtoParser::PbCommon::GetTypeAsString(field->type()) +
                " is not primitive");
      break;
  }
  return std::shared_ptr<SerializedObjectInterface>(sdprim);
}

#define ENCODE_REPEATED_PRITIMIVE(WIRE_TYPE, CPP_TYPE, WIRE_TYPENAME)        \
  WireFormat::EncodeTag(                                                     \
    field->tag(), WIRE_TYPE, sdprim->mutable_ostream());                     \
  const auto& repeated_field =                                               \
      *reinterpret_cast<const RepeatedField<CPP_TYPE>*>(field_addr);         \
  WireFormat::WriteUInt32(repeated_field.size(), sdprim->mutable_ostream()); \
  for (const auto& value: repeated_field) {                                  \
    WireFormat::Write##WIRE_TYPENAME(value, sdprim->mutable_ostream());      \
  }

std::shared_ptr<SerializedObjectInterface>
MessageReflection::CreateSerializedRepeatedPrimitive(
    const Message* message,
    const ProtoParser::MessageField* field)  const {

  SerializedPrimitive* sdprim = new SerializedPrimitive(field->type());
  const char* field_addr =
      reinterpret_cast<const char*>(message) + field->field_offset();
  switch (field->type()) {
    case ProtoParser::UINT32: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, uint32, UInt32)
      break;
    }
    case ProtoParser::UINT64: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, uint64, UInt64)
      break;
    }
    case ProtoParser::INT32: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, int32, SInt32)
      break;
    }
    case ProtoParser::INT64: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, int64, SInt64)
      break;
    }
    case ProtoParser::BOOL:{
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_VARIANT, bool, Bool)
      break;
    }
    case ProtoParser::DOUBLE: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_FIXD64, double, Double)
      break;
    }
    case ProtoParser::STRING: {
      ENCODE_REPEATED_PRITIMIVE(WireFormat::WIRETYPE_LENGTH_DIMITED, std::string, String)
      break;
    }
    case ProtoParser::ENUMTYPE: {
      // Cast enum value to uint32 by manipulating the underlying pointer.
      WireFormat::EncodeTag(
        field->tag(), WireFormat::WIRETYPE_VARIANT, sdprim->mutable_ostream());
      const auto repeated_field =
          reinterpret_cast<const RepeatedFieldBase*>(field_addr);
      WireFormat::WriteUInt32(repeated_field->size(), sdprim->mutable_ostream());
      for (uint32 i = 0; i < repeated_field->size(); i++) {
        uint32 value =
            *reinterpret_cast<const uint32*>(repeated_field->GetElementPtr(i));
        WireFormat::WriteUInt32(value, sdprim->mutable_ostream());
      }
      break;
    }
    default:
      throw std::runtime_error(
                "type " +
                ProtoParser::PbCommon::GetTypeAsString(field->type()) +
                " is not primitive");
      break;
  }
  return std::shared_ptr<SerializedObjectInterface>(sdprim);
}

bool MessageReflection::HasField(const Message* message, int tag) const {
  const char* has_bits =
    reinterpret_cast<const char*>(message) + has_bits_offset_;
  return (has_bits[tag / 8] & (0x1 << (tag % 8))) != 0;
}

}  // namespace proto