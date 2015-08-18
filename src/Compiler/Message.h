#ifndef MESSAGE_
#define MESSAGE_

#include <string>
#include <memory>
#include <map>
#include <set>

#include "Type.h"
#include "MessageField.h"
#include "EnumType.h"

namespace proto {
namespace ProtoParser {

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
  const std::set<int>& tag_set() const;
  int highest_tag() const { return highest_tag_; }
  MessageField* FindMessage(std::string name) const;
  EnumType* FindEnumType(std::string name) const;

  bool has_message_field() const { return has_message_field_; }
  bool has_repeated_field() const { return has_repeated_field_; }
  bool has_string_field() const { return has_string_field_; }

  std::vector<std::shared_ptr<MessageField>> mutable_fields_list();

  void SortFieldsByTag();

 protected:
  std::map<std::string, std::shared_ptr<MessageField>> fields_map_;
  std::vector<std::shared_ptr<MessageField>> fileds_list_;
  std::map<std::string, std::shared_ptr<EnumType>> enums_map_;
  std::set<int> tag_set_;

 private:
  bool has_message_field_ = false;
  bool has_string_field_ = false;
  bool has_repeated_field_ = false;
  int highest_tag_ = -1;
};

}  // namespace ProtoParser
}  // namespace Proto

#endif /* MESSAGE_ */