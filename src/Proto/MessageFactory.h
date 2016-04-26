#ifndef PROTO_MESSAGE_FACTORY_
#define PROTO_MESSAGE_FACTORY_

#include "map"
#include <memory>
#include "MessageReflection.h"

namespace proto {

class MessageFactory {
 public:
  static MessageFactory* instance();
  static void RegisterGeneratedMessage(MessageReflection* reflection);

  static int NumMessagesRegistered();

  static const MessageReflection* GetMessageReflection(std::string name);

 private:
  MessageFactory();
  // Disallow copy - message factory is singleton.
  FORBID_COPY_AND_ASSIGN(MessageFactory);

  static std::map<std::string, const MessageReflection*>& message_map();

  static MessageFactory* instance_;
  std::map<std::string, const MessageReflection*> message_map_;
};

}//  namespace proto

#endif  /* PROTO_MESSAGE_FACTORY_ */