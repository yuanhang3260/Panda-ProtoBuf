#include <iostream>

#include "MessageFactory.h"

namespace proto {

MessageFactory* MessageFactory::instance_;

MessageFactory::MessageFactory() {
}

// Message factory singleton.
MessageFactory* MessageFactory::instance() {
  if (!instance_) {
    instance_ = new MessageFactory();
  }
  return instance_;
}

std::map<std::string, const MessageReflection*>& MessageFactory::message_map() {
  return instance()->message_map_;
}

int MessageFactory::NumMessagesRegistered() {
  return instance()->message_map_.size();
}

void MessageFactory::RegisterGeneratedMessage(MessageReflection* reflection) {
  std::cout << "Reigstering Genereated Cpp Clsss "
            << reflection->descriptor()->FullNameWithPackagePrefix()
            << std::endl;
  (message_map())[reflection->descriptor()->FullNameWithPackagePrefix()] = reflection;
}

const MessageReflection*
MessageFactory::GetMessageReflection(std::string name) {
  if (message_map().find(name) != message_map().end()) {
    return (message_map()).at(name);
  }
  else {
    return NULL;
  }
}

}  //  namespace proto
