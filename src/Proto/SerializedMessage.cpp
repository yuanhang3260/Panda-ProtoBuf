#include "SerializedMessage.h"

namespace proto {

int SerializedMessage::CopyTo(char* buf) const {
  int offset = 0;
  for (const auto& field: fields_) {
    offset += field->CopyTo(buf + offset);
  }
  return offset;
}

void SerializedMessage::AddField(
    std::shared_ptr<SerializedObjectInterface> new_field) {
    fields_.push_back(new_field);
    size_ += new_field->size();
  }

}  // namespace proto
