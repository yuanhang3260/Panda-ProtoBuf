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
    default_value_(default_value) {
  type_name_ = GetTypeAsString(type);
}

MessageField::MessageField(FIELD_MODIFIER modifier,
                           FIELD_TYPE type,
                           std::string type_name,
                           std::string name,
                           int tag,
                           std::string default_value) :
    modifier_(modifier),
    type_(type),
    type_name_(type_name),
    name_(name),
    tag_(tag),
    default_value_(default_value) {
  if (type != MessageField::ENUMTYPE &&
      type != MessageField::MESSAGETYPE &&
      GetMessageFieldType(type_name_) != type) {
    type_name_ = GetTypeAsString(type);
  }
}

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

std::string
MessageField::GetModifierAsString(MessageField::FIELD_MODIFIER modifier) {
  if (modifier == MessageField::OPTIONAL) {
    return "optional";
  }
  if (modifier == MessageField::REQUIRED) {
    return "required";
  }
  if (modifier == MessageField::REPEATED) {
    return "repeated";
  }
  return "unknown_modifer";
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
  if (type == "bool") {
    return MessageField::BOOL;
  }
  return MessageField::UNDETERMINED;
}

std::string
MessageField::GetTypeAsString(MessageField::FIELD_TYPE type) {
  if (type == MessageField::INT32) {
    return "int32";
  }
  if (type == MessageField::INT64) {
    return "int64";
  }
  if (type == MessageField::UINT32) {
    return "uint32";
  }
  if (type == MessageField::UINT64) {
    return "uint64";
  }
  if (type == MessageField::DOUBLE) {
    return "double";
  }
  if (type == MessageField::STRING) {
    return "string";
  }
  if (type == MessageField::BOOL) {
    return "bool";
  }
  if (type == MessageField::ENUMTYPE) {
    return "enum";
  }
  if (type == MessageField::MESSAGETYPE) {
    return "message";
  }
  return "undermined";
}


}  // namespace Compiler
}  // namespace PandaProto
