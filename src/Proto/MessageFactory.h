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

  static int NumMessagesRegistered();

  static const MessageReflection* GetMessageReflection(std::string name);

 private:
  MessageFactory();
  ~MessageFactory();
  MessageFactory(const MessageFactory&) = delete;
  MessageFactory& operator=(const MessageFactory&) = delete;

  static std::map<std::string, std::shared_ptr<MessageReflection>>&
  message_map();

  static MessageFactory* instance_;
  std::map<std::string, std::shared_ptr<MessageReflection>> message_map_;
};

}//  namespace proto

#endif  /* PROTO_MESSAGE_FACTORY_ */