#include "Message.h"

namespace PandaProto {
namespace Compiler {

Message::Message(std::string name, std::string package) :
    name_(name),
    package_(package) {}

Message::~Message() {}

bool Message::AddField(std::shared_ptr<MessageField> field) {
  if (fields_map_.find(field->name()) != fields_map_.end()) {
    fprintf(stderr,
            "ERROR: field name \"%s\" already exisits in Message \"%s\"\n", 
            field->name().c_str(), name_.c_str());
    return false;
  }
  fields_map_[field->name()] = field;
  return false;
}

}  // namespace Compiler
}  // namespace PandaProto