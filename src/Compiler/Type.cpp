#include <iostream>
#include "Type.h"
#include "../Utility/Strings.h"

namespace proto {
namespace ProtoParser {

PbType::PbType(std::string name, std::string package) :
    name_(name),
    package_(package) {
  std::vector<std::string> result = StringUtils::Split(package_, '.');
  for (auto& pkg: result) {
    pkg_stack_.push_back(pkg);
  }
}

PbType::PbType(std::string name, std::string package, std::string message) :
    name_(name),
    package_(package),
    message_(message) {
  std::vector<std::string> result = StringUtils::Split(package_, '.');
  for (auto& pkg: result) {
    pkg_stack_.push_back(pkg);
  }
}

const std::string PbType::GeneratePackagePrefix(
    LANGUAGE lang, const std::vector<std::string>& pkg_stack) {
  std::string prefix = "", seprator = ".";
  switch (lang) {
    case CPP:
      seprator = "::";
      break;
    case JAVA:
    case PYTHON:
    default:
      break;
  }
  for (const auto& package: pkg_stack) {
    prefix += (package + seprator);
  }
  if (lang == CPP) {
    prefix = "::" + prefix;
  }
  return prefix;
}

std::string PbType::PackagePrefix(LANGUAGE lang) const {
  return GeneratePackagePrefix(lang, pkg_stack_);
}

std::string PbType::FullNameWithPackagePrefix(LANGUAGE lang) const {
  return PackagePrefix(lang) + name_;
}

}  // namespace ProtoParser
}  // namespace Proto







