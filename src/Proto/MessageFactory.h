#ifndef PROTO_MESSAGE_FACTORY_
#define PROTO_MESSAGE_FACTORY_

#include "map"
#include <memory>
#include "MessageReflection.h"

namespace proto {

class MessageFactory {
 public:
  static MessageFactory* instance();
  static void RegisterGeneratedMessage(
    std::shared_ptr<MessageReflection> reflection);

 private:
  MessageFactory();
  ~MessageFactory();
  MessageFactory(const MessageFactory&) = delete;
  MessageFactory& operator=(const MessageFactory&) = delete;

  static MessageFactory* instance_;
  static std::map<std::string, std::shared_ptr<MessageReflection>> message_map_;
};

}//  namespace proto

#endif  /* PROTO_MESSAGE_FACTORY_ */