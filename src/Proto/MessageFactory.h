#ifndef PROTO_MESSAGE_FACTORY_
#define PROTO_MESSAGE_FACTORY_

#include <map>
#include <memory>

#include "Proto/Descriptor.h"
#include "Proto/MessageReflection.h"

namespace proto {

class MessageFactory {
 public:
  static MessageFactory* instance();
  // Register a generated message reflection (takes ownership).
  static void RegisterGeneratedMessage(const MessageReflection* reflection);
  // Register a parsed proto file (takes ownership).
  static void RegisterParsedProtoFile(const ProtoFileDescriptor* file);

  static int NumMessagesRegistered();

  using ReflectionMap =
    std::map<std::string, std::shared_ptr<const MessageReflection>>;

  using ProtoFileSet = std::set<std::shared_ptr<const ProtoFileDescriptor>>;

  static const MessageReflection* GetMessageReflection(std::string name);
  //static const ProtoFileDescriptor* GetParsedProtoFile(std::string name);

 private:
  MessageFactory();
  // Disallow copy - message factory is singleton.
  FORBID_COPY_AND_ASSIGN(MessageFactory);

  static ReflectionMap& message_map();
  static ProtoFileSet& proto_files();
  static MessageFactory* instance_;

  ReflectionMap message_map_;
  ProtoFileSet proto_files_;
};

}//  namespace proto

#endif  /* PROTO_MESSAGE_FACTORY_ */