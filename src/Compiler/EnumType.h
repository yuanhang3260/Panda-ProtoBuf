#ifndef _ENUM_TYPE
#define _ENUM_TYPE

#include <string>
#include <vector>

namespace PandaProto {
namespace Compiler {


class EnumType {
 public:
  EnumType(std::string name, std::string package);
  EnumType(std::string name, std::string package, std::string message_);

  void AddEnumValue(std::string newenum);
  int NumberEnums() const;
  void Print() const;
  std::string name() const { return name_; }
  std::string package() const { return package_; }
  std::string message() const { return message_; }
  const std::vector<std::string>& enums() const { return enums_; }
  const std::vector<std::string>& pkg_stack() const { return pkg_stack_; }

 private:
  std::string name_;
  std::string package_;
  std::string message_ = "";
  std::vector<std::string> enums_;
  std::vector<std::string> pkg_stack_;
};


}  // PandaProto
}  // Compiler

#endif  /* _ENUM_TYPE */