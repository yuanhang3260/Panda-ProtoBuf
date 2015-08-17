#include <iostream>

#include "../Utility/Strings.h"
#include "EnumType.h"

namespace proto {
namespace ProtoParser {


EnumType::EnumType(std::string name, std::string package) :
    PbType(name, package) {
}

EnumType::EnumType(std::string name, std::string package, std::string message) :
    PbType(name, package + ("." + message), message) {
}

void EnumType::AddEnumValue(std::string newenum) {
  enums_.push_back(newenum);
  enums_set_.insert(newenum);
}

int EnumType::NumberEnums() const {
  return enums_.size();
}

bool EnumType::ContainsEnum(std::string enum_value) const {
  return enums_set_.find(enum_value) != enums_set_.end();
}

void EnumType::Print() const {
  std::string indent = message_.length() > 0? "  " : "";
  std::cout << indent << "enum " << name_ << " {" << std::endl;

  for (auto& enum_name: enums_) {
    std::cout << indent << "  " << enum_name << "," << std::endl;
  }
  std::cout << indent << "}" << std::endl;
}

}  // namespace ProtoParser
}  // namespace Proto