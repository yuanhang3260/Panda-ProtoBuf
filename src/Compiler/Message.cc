#include <iostream>
#include <algorithm>
#include <vector>

#include "Utility/Strings.h"
#include "Utility/Utils.h"
#include "Message.h"

namespace proto {
namespace ProtoParser {

Message::Message(std::string name, std::string package) :
    PbType(name, package) {
}

Message::~Message() {}

bool Message::AddField(std::shared_ptr<MessageField> field) {
  // Check duplicate tag.
  if (tag_set_.find(field->tag()) != tag_set_.end()) {
    fprintf(stderr,
            "ERROR: tag %d already exisits in Message, can't add field \"%s\"\n",
            field->tag(), field->name().c_str());
    return false;
  }
  tag_set_.insert(field->tag());
  highest_tag_ = Utils::Max(highest_tag_, field->tag());

  // Check duplicate name.
  if (fields_map_.find(field->name()) != fields_map_.end()) {
    fprintf(stderr,
            "ERROR: field name \"%s\" already exisits in Message \"%s\"\n", 
            field->name().c_str(), name_.c_str());
    return false;
  }

  fields_map_[field->name()] = field;
  tag_fields_map_[field->tag()] = field;
  fileds_list_.push_back(field);
  if (field->type() == MESSAGETYPE) {
    has_message_field_  = true;
  }
  if (field->type() == STRING) {
    has_string_field_  = true;
  }
  if (field->modifier() == REPEATED) {
    has_repeated_field_ = true;
  }
  return true;
}

bool Message::AddEnum(std::shared_ptr<EnumType> newenum) {
  if (enums_map_.find(newenum->name()) != enums_map_.end()) {
    fprintf(stderr,
            "ERROR: enum name \"%s\" already exisits in Message \"%s\"\n", 
            newenum->name().c_str(), name_.c_str());
    return false;
  }
  enums_map_[newenum->FullNameWithPackagePrefix(PYTHON)] = newenum;
  return true;
}

void Message::Print() {
  std::cout << "Message " << name_ << " in Package " << package_ << std::endl;
  // Print enums defined in this message.
  for (auto& e: enums_map_) {
    e.second->Print();
  }
  // Print message fields.
  for (auto& field: fileds_list_) {
    std::cout << "  " << MessageField::GetModifierAsString(field->modifier());
    std::cout << " " << field->type_name();
    std::cout << " " << field->name() << " = " << field->tag();
    if (field->default_value() != "") {
      std::cout << " [ default = " << field->default_value() << " ]";
    }
    std::cout << std::endl;
  }
}

bool FieldComparatorByTag(const std::shared_ptr<MessageField>& field1,
                          const std::shared_ptr<MessageField>& field2) {
  return field1->tag() < field2->tag();
}

void Message::SortFieldsByTag() {
  std::sort(fileds_list_.begin(), fileds_list_.end(), FieldComparatorByTag);
}

MessageField* Message::FindMessage(std::string name) const {
  if (fields_map_.find(name) == fields_map_.end()) {
    return NULL;
  }
  return fields_map_.at(name).get();
}

const MessageField* Message::FindFieldByTag(const unsigned int tag) const {
  if (tag_fields_map_.find(tag) == tag_fields_map_.end()) {
    return NULL;
  }
  return tag_fields_map_.at(tag).get();
}

const MessageField* Message::FindFieldByName(std::string field_name) const {
  if (fields_map_.find(field_name) == fields_map_.end()) {
    return NULL;
  }
  return fields_map_.at(field_name).get();
}

EnumType* Message::FindEnumType(std::string name) const {
  if (enums_map_.find(name) == enums_map_.end()) {
    return NULL;
  }
  return enums_map_.at(name).get();
}

const std::vector<std::shared_ptr<MessageField>>&
Message::fields_list() const {
  return fileds_list_;
}

std::vector<std::shared_ptr<MessageField>>
Message::mutable_fields_list() {
  return fileds_list_;
}

const std::map<std::string, std::shared_ptr<EnumType>>&
Message::enums_map() const {
  return enums_map_;
}

const std::set<int>& Message::tag_set() const {
  return tag_set_;
}

}  // namespace ProtoParser
}  // namespace Proto