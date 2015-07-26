#ifndef MESSAGE_
#define MESSAGE_

#include <string>
#include <memory>
#include <map>

#include "MessageField.h"
#include "EnumType.h"

namespace PandaProto {
namespace Compiler {

class Message {
 public:
  Message(std::string name, std::string package);
  virtual ~Message();

  bool AddField(std::shared_ptr<MessageField> field);
  bool AddEnum(std::shared_ptr<EnumType> newenum);
  void Print();

  std::string name() const { return name_; }
  std::string package() const { return package_; }
  const std::vector<std::shared_ptr<MessageField>>& fields_list() const;
  const std::map<std::string, std::shared_ptr<EnumType>>& enums_map() const;
  const std::vector<std::string>& pkg_stack() const;
  MessageField* FindMessage(std::string name) const;
  EnumType* FindEnumType(std::string name) const;

 private:
  std::string name_;
  std::string package_ = "";  // namespace
  std::vector<std::string> pkg_stack_;
  std::map<std::string, std::shared_ptr<MessageField>> fields_map_;
  std::vector<std::shared_ptr<MessageField>> fileds_list_;
  std::map<std::string, std::shared_ptr<EnumType>> enums_map_;
};

}  // namespace Compiler
}  // namespace PandaProto

#endif /* MESSAGE_ */