#ifndef _ENUM_TYPE
#define _ENUM_TYPE

#include <string>
#include <vector>

#include "Type.h"

namespace PandaProto {
namespace Compiler {


class EnumType : public PbType {
 public:
  EnumType(std::string name, std::string package);
  EnumType(std::string name, std::string package, std::string message_);

  FIELD_TYPE type() override { return ENUMTYPE; }

  void AddEnumValue(std::string newenum);
  int NumberEnums() const;
  void Print() const;
  const std::vector<std::string>& enums() const { return enums_; }

 protected:
  std::vector<std::string> enums_;
};


}  // PandaProto
}  // Compiler

#endif  /* _ENUM_TYPE */