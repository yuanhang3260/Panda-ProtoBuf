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

std::map<std::string, std::shared_ptr<MessageReflection>>&
MessageFactory::message_map() {
  return instance()->message_map_;
}

int MessageFactory::NumMessagesRegistered() {
  return instance()->message_map_.size();
}

void MessageFactory::RegisterGeneratedMessage(
    std::shared_ptr<MessageReflection> reflection) {
  std::cout << "Reigstering Genereated Cpp Clsss "
            << reflection->descriptor()->FullNameWithPackagePrefix(ProtoParser::CPP)
            << std::endl;
  (message_map())[reflection->descriptor()->FullNameWithPackagePrefix(ProtoParser::CPP)] = reflection;
}

const MessageReflection*
MessageFactory::GetMessageReflection(std::string name) {
  if (message_map().find(name) != message_map().end()) {
    return ((message_map())[name]).get();
  }
  else {
    return NULL;
  }
}

}  //  namespace proto
