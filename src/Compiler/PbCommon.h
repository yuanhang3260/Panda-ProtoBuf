#ifndef PB_COMMON_
#define PB_COMMON_

#include <string>

namespace PandaProto {
namespace Compiler {

enum FIELD_TYPE {
  UNDETERMINED,
  INT32,
  INT64,
  UINT32,
  UINT64,
  DOUBLE,
  BOOL,
  STRING,
  ENUMTYPE,
  MESSAGETYPE,
};

class PbCommon {
 public:
  static FIELD_TYPE GetMessageFieldType(std::string type);
  static std::string GetTypeAsString(FIELD_TYPE type);
};

}  // namespace PandaProto
}  // namespace Compiler

#endif  /* PB_COMMON_ */