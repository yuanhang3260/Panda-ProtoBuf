#ifndef PB_COMMON_
#define PB_COMMON_

#include <string>

#include "../Proto/Common.h"

namespace proto {
namespace ProtoParser {

enum LANGUAGE {
  CPP,
  PYTHON,
  JAVA,
  UNKNOWN_LANGUAGE,
};

class PbCommon {
 public:
  static FIELD_TYPE GetMessageFieldType(std::string type);
  static std::string GetTypeAsString(FIELD_TYPE type);
};

}  // namespace ProtoParser
}  // namespace Proto

#endif  /* PB_COMMON_ */