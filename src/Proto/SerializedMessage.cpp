#include <string.h>

#include "SerializedMessage.h"

namespace proto {

int SerializedMessage::CopyTo(char* buf) const {
  std::cout << "meta_data_ size = " << meta_data_.size() << std::endl;
  memcpy(buf, meta_data_.CharArray(), meta_data_.size());
  int offset = meta_data_.size();
  std::cout << "has " << fields_.size() << " fields" << std::endl;
  for (const auto& field: fields_) {
    offset += field->CopyTo(buf + offset);
  }
  if (offset != size_) {
    std::runtime_error("Message object size_ mismatch");
  }
  return offset;
}

void SerializedMessage::AddField(
    std::shared_ptr<SerializedObjectInterface> new_field) {
  std::cout << "Adding field ... " << new_field.get() << std::endl;
  std::cout << "new_field->size() = " << new_field->size() << std::endl;
  fields_.push_back(new_field);
  size_ += new_field->size();
}

void SerializedMessage::ReCalculateSize() {
  size_ = 0;
  for (const auto& field: fields_) {
    size_ += field->size();
  }
  size_ += meta_data_.size();
}

}  // namespace proto
