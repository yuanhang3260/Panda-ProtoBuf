#include "MessageReflection.h"

namespace proto {

MessageReflection::MessageReflection(
    std::shared_ptr<::proto::ProtoParser::Message> message_descirptor,
    Message* defatult_instance) :
    message_descirptor_(message_descirptor),
    defatult_instance_(defatult_instance) {
}

}  // namespace proto