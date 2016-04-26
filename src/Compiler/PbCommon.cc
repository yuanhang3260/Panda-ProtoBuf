#include "PbCommon.h"
#include "Utility/Strings.h"

namespace proto {
namespace ProtoParser {

FIELD_TYPE
PbCommon::GetMessageFieldType(std::string type) {
  if (type == "int32") {
    return INT32;
  }
  if (type == "int64") {
    return INT64;
  }
  if (type == "uint32") {
    return UINT32;
  }
  if (type == "uint64") {
    return UINT64;
  }
  if (type == "double") {
    return DOUBLE;
  }
  if (type == "string") {
    return STRING;
  }
  if (type == "bool") {
    return BOOL;
  }
  return UNDETERMINED;
}

std::string
PbCommon::GetTypeAsString(FIELD_TYPE type) {
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
  if (type == BOOL) {
    return "bool";
  }
  return "undermined";
}

}  // namespace ProtoParser
}  // namespace Proto