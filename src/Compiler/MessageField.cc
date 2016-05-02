#include "MessageField.h"

namespace proto {
namespace ProtoParser {

MessageField::MessageField(FieldLabel modifier,
                           FieldType type,
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
  has_user_default_value_ = false;
  if (modifier_ == REPEATED) {
    default_value  = "";
    return;
  }
  if (default_value_.empty()) {
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
    }
  }
  else {
    has_user_default_value_ = true;
    if (type_ == STRING) {
      default_value_ = "\"" + default_value_ + "\"";
    }
  }
}

MessageField::~MessageField() {}

FieldLabel MessageField::GetMessageFieldModifier(std::string line) {
  if (line == "optional") {
    return OPTIONAL;
  }
  if (line == "required") {
    return REQUIRED;
  }
  if (line == "repeated") {
    return REPEATED;
  }
  return UNKNOWN_MODIFIER;
}

std::string MessageField::GetModifierAsString(FieldLabel modifier) {
  if (modifier == OPTIONAL) {
    return "optional";
  }
  if (modifier == REQUIRED) {
    return "required";
  }
  if (modifier == REPEATED) {
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
  return modifier_ == REPEATED;
}

bool MessageField::IsSingularType() const {
  return modifier_ != REPEATED;
}

bool MessageField::IsSingularNumericType() const {
  return modifier_ != REPEATED &&
         type_ != MESSAGETYPE &&
         type_ != STRING;
}

bool MessageField::IsSingularStringType() const {
  return modifier_ != REPEATED &&
         type_ == STRING;
}

bool MessageField::IsSingularMessageType() const {
  return modifier_ != REPEATED &&
         type_ == MESSAGETYPE;
}

bool MessageField::IsRepeatedNumericType() const {
  return modifier_ == REPEATED &&
         type_ != MESSAGETYPE &&
         type_ != STRING;
}

bool MessageField::IsRepeatedStringType() const {
  return modifier_ == REPEATED &&
         type_ == STRING;
}

bool MessageField::IsRepeatedMessageType() const {
  return modifier_ == REPEATED &&
         type_ == MESSAGETYPE;
}


}  // namespace ProtoParser
}  // namespace Proto
