#include <stdlib.h>
#include <string.h>

#include "SerializedPrimitive.h"

namespace proto {

int SerializedPrimitive::CopyTo(char* buf) const {
  memcpy(buf, internal_buf_.CharArray(), internal_buf_.size());
  return internal_buf_.size();
}


}  // namespace proto
