#ifndef MESSAGE_
#define MESSAGE_

#include <string>
#include <memory>
#include <map>

#include "MessageField.h"

namespace PandaProto {
namespace Compiler {

class Message {
 public:
  Message(std::string name, std::string package);
  virtual ~Message();

  bool AddField(std::shared_ptr<MessageField> field);
  void Print();

  std::string name() const { return name_; }
  std::string package() const { return package_; }

 private:
  std::string name_;
  std::string package_ = "";  // namespace
  std::map<std::string, std::shared_ptr<MessageField>> fields_map_;
  std::vector<std::shared_ptr<MessageField>> fileds_list_;
};

}  // namespace Compiler
}  // namespace PandaProto

#endif /* MESSAGE_ */