#include "MessageField.h"

namespace PandaProto {
namespace Compiler {

MessageField::MessageField(FIELD_MODIFIER modifier,
                           FIELD_TYPE type,
                           PbType* type_class,
                           std::string name,
                           int tag,
                           std::string default_value) :
    modifier_(modifier),
    type_(type),
    type_class_(type_class),
    name_(name),
    tag_(tag),
    default_value_(default_value) {
  if (!type_class_) {
    type_name_ = PbCommon::GetTypeAsString(type);
  }
  else {
    type_name_ = type_class_->name();
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

bool MessageField::IsPrimitiveType() const {
  return type_ != STRING && type_ != MESSAGETYPE;
}

bool MessageField::IsSingularNumericType() const {
  return modifier_ != MessageField::REPEATED &&
         type_ != MESSAGETYPE &&
         type_ != STRING;
}

bool MessageField::IsSingularStringType() const {
  return modifier_ != MessageField::REPEATED &&
         type_ == STRING;
}

bool MessageField::IsSingularMessageType() const {
  return modifier_ != MessageField::REPEATED &&
         type_ == MESSAGETYPE;
}

bool MessageField::IsRepeatedType() const {
  return modifier_ == MessageField::REPEATED;
}


}  // namespace Compiler
}  // namespace PandaProto
