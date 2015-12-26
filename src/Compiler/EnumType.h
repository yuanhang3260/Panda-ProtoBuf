#ifndef ENUM_TYPE_
#define ENUM_TYPE_

#include <string>
#include <vector>
#include <set>

#include "Type.h"

namespace proto {
namespace ProtoParser {


class EnumType : public PbType {
 public:
  EnumType(std::string name, std::string package);
  EnumType(std::string name, std::string package, std::string message_);

  FIELD_TYPE type() override { return ENUMTYPE; }

  void AddEnumValue(std::string newenum);
  int NumberEnums() const;
  bool ContainsEnum(std::string enum_value) const;
  void Print() const;
  const std::vector<std::string>& enums() const { return enums_; }
  std::string EnumValueAsString(int value) const;

 protected:
  std::vector<std::string> enums_;
  std::set<std::string> enums_set_;
};


}  // namespace ProtoParser
}  // namespace Proto

#endif  /* ENUM_TYPE_ */