#ifndef PROTO_COMMON_
#define PROTO_COMMON_

#include <string>

namespace proto {

enum FieldLabel {
  UNKNOWN_MODIFIER,
  OPTIONAL,
  REQUIRED,
  REPEATED,
};

enum FieldType {
  UNDETERMINED,
  INT32,
  INT64,
  UINT32,
  UINT64,
  DOUBLE,
  BOOL,
  CHAR,
  STRING,
  ENUMTYPE,
  MESSAGETYPE,
  SERVICETYPE,
};

std::string LabelAsString(FieldLabel label);
std::string TypeAsString(FieldType type);

using FIELD_TYPE = FieldType;
using FIELD_MODIFIER = FieldLabel;
using string = std::string;

}  // namespace proto


#endif  // PROTO_COMMON_
