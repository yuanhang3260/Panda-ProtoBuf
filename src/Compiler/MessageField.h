#ifndef MESSAGE_FIELD_
#define MESSAGE_FIELD_

#include <string>

namespace PandaProto {
namespace Compiler {

class MessageField {
 public:
  enum FIELD_MODIFIER {
    UNKNOWN_MODIFIER,
    OPTIONAL,
    REQUIRED,
    REPEATED,
  };

  enum FIELD_TYPE {
    UNDETERMINED,
    INT32,
    INT64,
    UINT32,
    UINT64,
    DOUBLE,
    STRING,
    BOOL,
    ENUMTYPE,
    MESSAGETYPE,
  };

  MessageField(FIELD_MODIFIER modifier, FIELD_TYPE type, std::string name,
               int tag, std::string default_value);
  MessageField(FIELD_MODIFIER modifier, FIELD_TYPE type, std::string type_name,
               std::string name, int tag, std::string default_value);
  virtual ~MessageField();

  static FIELD_MODIFIER GetMessageFieldModifier(std::string modifier);
  static std::string GetModifierAsString(MessageField::FIELD_MODIFIER modifier);
  static FIELD_TYPE GetMessageFieldType(std::string type);
  static std::string GetTypeAsString(MessageField::FIELD_TYPE type);

  FIELD_MODIFIER modifier() const { return modifier_; }
  FIELD_TYPE type() const { return type_; }
  std::string name() const { return name_; }
  int tag() const { return tag_; }
  std::string default_value() const { return default_value_; }
  std::string type_name() const { return type_name_; }

  void set_type_name(std::string type_name) { type_name_ = type_name; }

 private:
  FIELD_MODIFIER modifier_;
  FIELD_TYPE type_;
  std::string type_name_;
  std::string name_;
  int tag_;
  std::string default_value_;
};

}  // namespace Compiler
}  // namespace PandaProto

#endif /* MESSAGE_FIELD_ */