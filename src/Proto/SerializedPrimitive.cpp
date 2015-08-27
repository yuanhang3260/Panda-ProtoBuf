#include <stdlib.h>
#include <string.h>

#include "SerializedPrimitive.h"

namespace proto {

int SerializedMessage::CopyTo(char* buf) {
  memcpy(buf, internal_buf_.CharArray(), internal_buf_.size());
  return internal_buf_.size();
}

void SerializedMessage::Encode(const char* raw_field) {
  
}

}  // namespace proto
