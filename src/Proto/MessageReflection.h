#ifndef GENERATED_MESSAGE_REFLECTION_
#define GENERATED_MESSAGE_REFLECTION_

#include <memory>

#include "../Compiler/Message.h"
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
      Message* defatult_instance);

 private:
  std::shared_ptr<::proto::ProtoParser::Message> message_descirptor_;
  Message* defatult_instance_;
};

}  // namespace proto

#endif  /* GENERATED_MESSAGE_REFLECTION_ */