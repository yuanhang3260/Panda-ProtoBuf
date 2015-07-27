#include "Type.h"
#include "../Utility/Strings.h"

namespace PandaProto {
namespace Compiler {

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

}  // PandaProto
}  // Compiler







