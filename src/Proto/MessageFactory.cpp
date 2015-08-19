#include "MessageFactory.h"

namespace proto {

MessageFactory* MessageFactory::instance_ = NULL;

MessageFactory::MessageFactory() {
}

MessageFactory::~MessageFactory() {
}

MessageFactory* MessageFactory::instance() {
  if (!instance_) {
    instance_ = new MessageFactory();
  }
  return instance_;
}

void MessageFactory::RegisterGeneratedMessage(
    std::shared_ptr<MessageReflection> reflection) {
  message_map_[reflection->descriptor()->name()] = reflection;
}

}  //  namespace proto
