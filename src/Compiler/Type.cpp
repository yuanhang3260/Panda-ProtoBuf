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

}  // namespace ProtoParser
}  // namespace Proto







