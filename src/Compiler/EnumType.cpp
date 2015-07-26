#include <iostream>

#include "../Utility/Strings.h"
#include "EnumType.h"

namespace PandaProto {
namespace Compiler {


EnumType::EnumType(std::string name, std::string package) :
    name_(name),
    package_(package) {
  std::vector<std::string> result = StringUtils::Split(package_, '.');
  for (auto& pkg: result) {
    pkg_stack_.push_back(pkg);
  }
}

EnumType::EnumType(std::string name, std::string package, std::string message) :
    name_(name),
    package_(package),
    message_(message) {
  std::vector<std::string> result = StringUtils::Split(package_, '.');
  for (auto& pkg: result) {
    pkg_stack_.push_back(pkg);
  }
}

void EnumType::AddEnumValue(std::string newenum) {
  enums_.push_back(newenum);
}

int EnumType::NumberEnums() const {
  return enums_.size();
}

void EnumType::Print() const {
  std::string indent = message_.length() > 0? "  " : "";
  std::cout << indent << "enum " << name_ << " {" << std::endl;

  for (auto& enum_name: enums_) {
    std::cout << indent << "  " << enum_name << "," << std::endl;
  }
  std::cout << indent << "}" << std::endl;
}


}  // PandaProto
}  // Compiler