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
    ENUM,
    MESSAGE,
  };

  MessageField(FIELD_MODIFIER modifier, FIELD_TYPE type, std::string name,
               int tag, std::string default_value);
  virtual ~MessageField();

  static FIELD_MODIFIER GetMessageFieldModifier(std::string modifier);
  static FIELD_TYPE GetMessageFieldType(std::string type);

  std::string name() const { return name_; }
  FIELD_TYPE type() const { return type_; }
  int tag() const { return tag_; }

 private:
  FIELD_MODIFIER modifier_;
  FIELD_TYPE type_;
  std::string name_;
  int tag_;
  std::string default_value_;
};

}  // namespace Compiler
}  // namespace PandaProto

#endif /* MESSAGE_FIELD_ */