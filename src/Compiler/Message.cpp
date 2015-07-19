#include <iostream>
#include <algorithm>
#include <vector>

#include "Message.h"

namespace PandaProto {
namespace Compiler {

Message::Message(std::string name, std::string package) :
    name_(name),
    package_(package) {}

Message::~Message() {}

bool Message::AddField(std::shared_ptr<MessageField> field) {
  if (fields_map_.find(field->name()) != fields_map_.end()) {
    fprintf(stderr,
            "ERROR: field name \"%s\" already exisits in Message \"%s\"\n", 
            field->name().c_str(), name_.c_str());
    return false;
  }
  fields_map_[field->name()] = field;
  fileds_list_.push_back(field);
  return true;
}

bool FieldComparator(const std::shared_ptr<MessageField>& field1,
                     const std::shared_ptr<MessageField>& field2) {
  return field1->tag() < field2->tag();
}

void Message::Print() {
  std::cout << "\nMessage " << name_ << "in Package " << package_ << std::endl;
  std::sort(fileds_list_.begin(), fileds_list_.end(), FieldComparator);
  for (auto& field: fileds_list_) {
    std::cout << "  " << MessageField::GetModifierAsString(field->modifier());
    std::cout << " " << MessageField::GetTypeAsString(field->type());
    std::cout << " " << field->name() << " = " << field->tag();
    if (field->default_value() != "") {
      std::cout << " [ default = " << field->default_value() << " ]";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

}  // namespace Compiler
}  // namespace PandaProto