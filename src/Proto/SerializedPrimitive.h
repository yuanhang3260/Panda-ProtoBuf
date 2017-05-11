#ifndef SERIALIZED_PRIMITIVE_
#define SERIALIZED_PRIMITIVE_

#include "Compiler/PbCommon.h"
#include "SerializedObjectInterface.h"
#include "Strings/StringBuilder.h"

namespace proto {

class SerializedPrimitive: public SerializedObjectInterface {
 public:
  SerializedPrimitive(FieldType type): type_(type) {}
  SerializedPrimitive(const SerializedPrimitive& other) = delete;
  SerializedPrimitive operator=(const SerializedPrimitive& other) = delete;

  int CopyTo(char* buf) const override;

  ::Strings::StringBuilder* mutable_ostream() { return &internal_buf_; }

  unsigned int size() const override { return internal_buf_.size(); }

 private:
  FieldType type_;
  ::Strings::StringBuilder internal_buf_;
};

}  // namespace proto

#endif  /* SERIALIZED_PRIMITIVE_ */