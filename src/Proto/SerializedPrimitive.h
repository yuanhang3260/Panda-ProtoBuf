#ifndef SERIALIZED_PRIMITIVE
#define SERIALIZED_PRIMITIVE

#include "../Utility/StringBuilder.h"
#include "../Compiler/PbCommon.h"
#include "SerializedObjectInterface.h"

namespace proto {

class SerializedPrimitive: public SerializedObjectInterface {
 public:
  SerializedPrimitive(::proto::ProtoParser::FIELD_TYPE type): type_(type) {}

  int CopyTo(char* buf) const override;

  int size() const override { return internal_buf_.size(); }

 private:
  ::proto::ProtoParser::FIELD_TYPE type_;
  ::Utility::StringBuilder internal_buf_;
};

}  // namespace proto

#endif  /* SERIALIZED_PRIMITIVE */