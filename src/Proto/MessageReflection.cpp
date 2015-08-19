#include "MessageReflection.h"

namespace proto {

MessageReflection::MessageReflection(
    std::shared_ptr<::proto::ProtoParser::Message> message_descirptor,
    Message* defatult_instance) :
    message_descirptor_(message_descirptor),
    defatult_instance_(defatult_instance) {
}

const ::proto::ProtoParser::Message*
MessageReflection::descriptor() {
  return message_descirptor_.get();
}

const Message* MessageReflection::defatult_instance() {
  return defatult_instance_;
}

}  // namespace proto