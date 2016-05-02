#include "Proto/Common.h"

namespace proto {

std::string GetLabelAsString(FieldLabel label) {
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

}  // namespace proto
