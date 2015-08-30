#ifndef SERIALIZED_PRIMITIVE
#define SERIALIZED_PRIMITIVE

#include "../Utility/StringBuilder.h"
#include "../Compiler/PbCommon.h"
#include "SerializedObjectInterface.h"

namespace proto {

class SerializedPrimitive: public SerializedObjectInterface {
 public:
  SerializedPrimitive(::proto::ProtoParser::FIELD_TYPE type): type_(type) {}
  SerializedPrimitive(const SerializedPrimitive& other) = delete;
  SerializedPrimitive operator=(const SerializedPrimitive& other) = delete;

  int CopyTo(char* buf) const override;

  ::Utility::StringBuilder* mutable_ostream() { return &internal_buf_; }

  int size() const override { return internal_buf_.size(); }

 private:
  ::proto::ProtoParser::FIELD_TYPE type_;
  ::Utility::StringBuilder internal_buf_;
};

}  // namespace proto

#endif  /* SERIALIZED_PRIMITIVE */