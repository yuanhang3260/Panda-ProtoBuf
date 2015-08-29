#ifndef GENERATED_MESSAGE_REFLECTION_
#define GENERATED_MESSAGE_REFLECTION_

#include <memory>
#include <stdexcept>

#include "../Compiler/Message.h"
#include "../Compiler/PbCommon.h"
#include "SerializedMessage.h"
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

 private:
  std::shared_ptr<::proto::ProtoParser::Message> message_descirptor_;
  Message* defatult_instance_;
  int has_bits_offset_;
};

}  // namespace proto

#endif  /* GENERATED_MESSAGE_REFLECTION_ */