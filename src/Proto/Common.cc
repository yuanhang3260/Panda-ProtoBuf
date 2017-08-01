#include "Proto/Common.h"

namespace proto {

std::string LabelAsString(FieldLabel label) {
  if (label == OPTIONAL) {
    return "optional";
  }
  if (label == REQUIRED) {
    return "required";
  }
  if (label == REPEATED) {
    return "repeated";
  }
  return "unknown_modifer";
}

std::string TypeAsString(FieldType type) {
  if (type == INT32) {
    return "int32";
  }
  if (type == INT64) {
    return "int64";
  }
  if (type == UINT32) {
    return "uint32";
  }
  if (type == UINT64) {
    return "uint64";
  }
  if (type == DOUBLE) {
    return "double";
  }
  if (type == STRING) {
    return "string";
  }
  if (type == CHAR) {
    return "char";
  }
  if (type == BOOL) {
    return "bool";
  }
  if (type == ENUMTYPE) {
    return "enum";
  }
  if (type == MESSAGETYPE) {
    return "message";
  }
  return "undermined";
}

}  // namespace proto
