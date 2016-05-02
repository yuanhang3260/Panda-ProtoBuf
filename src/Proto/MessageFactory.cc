#include <iostream>

#include "Proto/MessageFactory.h"
#include "Proto/Descriptors_internal.h"

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

MessageFactory::ReflectionMap& MessageFactory::message_map() {
  return instance()->message_map_;
}

MessageFactory::ProtoFileMap& MessageFactory::file_map() {
  return instance()->file_map_;
}

int MessageFactory::NumMessagesRegistered() {
  return instance()->message_map_.size();
}

void MessageFactory::RegisterGeneratedMessage(
                          const MessageReflection* reflection) {
  std::cout << "Reigstering Genereated Cpp Clsss "
            << reflection->descriptor()->full_name()
            << std::endl;
  message_map().emplace(reflection->descriptor()->full_name(),
                        std::shared_ptr<const MessageReflection>(reflection));
}

const MessageReflection*
MessageFactory::GetMessageReflection(std::string name) {
  if (message_map().find(name) != message_map().end()) {
    return message_map().at(name).get();
  }
  else {
    return nullptr;
  }
}

void MessageFactory::RegisterParsedProtoFile(
                          const ProtoFileDescriptor* file) {
  std::cout << "Reigstering parsed proto file "
            << file->path() << std::endl;
  file_map().emplace(file->path(),
                     std::shared_ptr<const ProtoFileDescriptor>(file));
}

const ProtoFileDescriptor*
MessageFactory::GetParsedProtoFile(std::string name) {
  if (file_map().find(name) != file_map().end()) {
    return file_map().at(name).get();
  }
  else {
    return nullptr;
  }
}

}  //  namespace proto
