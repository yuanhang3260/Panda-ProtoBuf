#ifndef GENERATED_MESSAGE_REFLECTION_
#define GENERATED_MESSAGE_REFLECTION_

#include <memory>
#include <stdexcept>

#include "../Compiler/Message.h"
#include "../Compiler/PbCommon.h"
#include "SerializedMessage.h"
#include "SerializedPrimitive.h"
#include "Message.h"

namespace proto {

#define PROTO_MESSAGE_FIELD_OFFSET(TYPE, FIELD)                \
  static_cast<int>(                                            \
      reinterpret_cast<const char*>(                           \
          &reinterpret_cast<const TYPE*>(16)->FIELD) -         \
      reinterpret_cast<const char*>(16))

#define CHECK(condition, error_msg)            \
  if (!condition) {                            \
    throw std::runtime_error(error_msg);       \
  }

class MessageReflection {
 public:
  MessageReflection(
      std::shared_ptr<::proto::ProtoParser::Message> message_descirptor,
      Message* defatult_instance,
      int has_bits_offset);

  const ::proto::ProtoParser::Message* descriptor();
  const Message* defatult_instance();
  SerializedMessage* Serialize(const Message* message) const;

 private:
  bool HasField(const Message* message, int tag) const;

  // Create a serialized singular message object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedSingularMessage(
    const Message* message,
    const ProtoParser::MessageField* field)  const;

  // Create a serialized repeated message object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedRepeatedMessage(
    const Message* message,
    const ProtoParser::MessageField* field)  const;

  // Create a serialized singular primitive object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedSingularPrimitive(
    const Message* message,
    const ProtoParser::MessageField* field) const;

  // Create a repeated singular primitive object.
  std::shared_ptr<SerializedObjectInterface>
  CreateSerializedRepeatedPrimitive(
    const Message* message,
    const ProtoParser::MessageField* field) const;

 private:
  std::shared_ptr<::proto::ProtoParser::Message> message_descirptor_;
  Message* defatult_instance_;
  int has_bits_offset_;
};

}  // namespace proto

#endif  /* GENERATED_MESSAGE_REFLECTION_ */