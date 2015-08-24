#include <iostream>

#include "MessageFactory.h"

namespace proto {

MessageFactory* MessageFactory::instance_;

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

int MessageFactory::NumMessagesRegistered() {
  return instance()->message_map_.size();
}

void MessageFactory::RegisterGeneratedMessage(
    std::shared_ptr<MessageReflection> reflection) {
  std::cout << "Reigstering Genereated Clsss "
            << reflection->descriptor()->name() << std::endl;
  MessageFactory::instance()->message_map_[
    reflection->descriptor()->name()] =reflection;
}

}  //  namespace proto
