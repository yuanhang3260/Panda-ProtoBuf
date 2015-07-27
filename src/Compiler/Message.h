#ifndef MESSAGE_
#define MESSAGE_

#include <string>
#include <memory>
#include <map>

#include "Type.h"
#include "MessageField.h"
#include "EnumType.h"

namespace PandaProto {
namespace Compiler {

class Message : public PbType {
 public:
  Message(std::string name, std::string package);
  virtual ~Message();

  bool AddField(std::shared_ptr<MessageField> field);
  bool AddEnum(std::shared_ptr<EnumType> newenum);
  void Print();

  FIELD_TYPE type() override { return MESSAGETYPE; }

  const std::vector<std::shared_ptr<MessageField>>& fields_list() const;
  const std::map<std::string, std::shared_ptr<EnumType>>& enums_map() const;
  MessageField* FindMessage(std::string name) const;
  EnumType* FindEnumType(std::string name) const;

 protected:
  std::map<std::string, std::shared_ptr<MessageField>> fields_map_;
  std::vector<std::shared_ptr<MessageField>> fileds_list_;
  std::map<std::string, std::shared_ptr<EnumType>> enums_map_;
};

}  // namespace Compiler
}  // namespace PandaProto

#endif /* MESSAGE_ */