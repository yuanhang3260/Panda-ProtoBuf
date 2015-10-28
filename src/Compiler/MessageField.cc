#include "MessageField.h"

namespace proto {
namespace ProtoParser {

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

  // Set default value.
  if (modifier_ != REPEATED) {
    if (default_value_.empty()) {
      has_user_default_value_ = false;
       // They are system default value for different types.
      if (IsPrimitiveType()) {
        if (type_ == BOOL) {
          default_value_ = "false";
        }
        else if (type_ == ENUMTYPE) {
          default_value_ = (static_cast<EnumType*>(type_class))->enums()[0];
        }
        else {
          default_value_ = "0";
        }
      }
      else if (type_ == STRING) {
        default_value_ = "\"\"";
      }
      else {  // Message Type
        default_value_ = "nullptr";
        // we force message type has default value = nullptr.
        has_user_default_value_ = true;
      }
    }
    else {
      if (type_ == STRING) {
        default_value_ = "\"" + default_value_ + "\"";
      }
      has_user_default_value_ = true;
    }
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
MessageField::GetModifierAsString(FIELD_MODIFIER modifier) {
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

bool MessageField::IsMessageType() const {
  return type_ == MESSAGETYPE;
}

bool MessageField::IsRepeatedType() const {
  return modifier_ == MessageField::REPEATED;
}

bool MessageField::IsSingularType() const {
  return modifier_ != MessageField::REPEATED;
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

bool MessageField::IsRepeatedNumericType() const {
  return modifier_ == MessageField::REPEATED &&
         type_ != MESSAGETYPE &&
         type_ != STRING;
}

bool MessageField::IsRepeatedStringType() const {
  return modifier_ == MessageField::REPEATED &&
         type_ == STRING;
}

bool MessageField::IsRepeatedMessageType() const {
  return modifier_ == MessageField::REPEATED &&
         type_ == MESSAGETYPE;
}


}  // namespace ProtoParser
}  // namespace Proto
