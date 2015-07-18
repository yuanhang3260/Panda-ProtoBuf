#include "MessageField.h"

namespace PandaProto {
namespace Compiler {

MessageField::MessageField(FIELD_MODIFIER modifier,
                           FIELD_TYPE type,
                           std::string name,
                           int tag,
                           std::string default_value) :
    modifier_(modifier),
    type_(type),
    name_(name),
    tag_(tag),
    default_value_(default_value) {}

MessageField::~MessageField() {}

MessageField::FIELD_MODIFIER
MessageField::GetMessageFieldModifier(std::string line) {
  if (line == "optional") {
    return MessageField::OPTIONAL;
  }
  if (line == "required") {
    return MessageField::REQUIRED;
  }
  if (line == "repeated") {
    return MessageField::REPEATED;
  }
  return MessageField::UNKNOWN_MODIFIER;
}

MessageField::FIELD_TYPE
MessageField::GetMessageFieldType(std::string type) {
  if (type == "int32") {
    return MessageField::INT32;
  }
  if (type == "int64") {
    return MessageField::INT64;
  }
  if (type == "uint32") {
    return MessageField::INT32;
  }
  if (type == "uint64") {
    return MessageField::INT64;
  }
  if (type == "double") {
    return MessageField::DOUBLE;
  }
  if (type == "string") {
    return MessageField::STRING;
  }
  return MessageField::UNDETERMINED;
}

}  // namespace Compiler
}  // namespace PandaProto
