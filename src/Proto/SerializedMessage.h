#ifndef SERIALIZED_OBJECT_
#define SERIALIZED_OBJECT_

#include <vector>
#include <memory>

#include "../Utility/StringBuilder.h"
#include "SerializedObjectInterface.h"

namespace proto {

class SerializedMessage: public SerializedObjectInterface {
 public:
  SerializedMessage() {}
  SerializedMessage(const bool is_repeated) : is_repeated_(is_repeated) {}
  SerializedMessage(const SerializedMessage& other) = delete;
  SerializedMessage& operator=(const SerializedMessage& other) = delete;
  ~SerializedMessage() { std::cout << "deleting SerializedMessage" << std::endl; }

  int CopyTo(char* buf) const override;

  void AddField(std::shared_ptr<SerializedObjectInterface> new_field);
  ::Utility::StringBuilder* meta_data() { return &meta_data_; }
  void ReCalculateSize();

 private:
  std::vector<std::shared_ptr<SerializedObjectInterface>> fields_;
  bool is_repeated_ = false;
  ::Utility::StringBuilder meta_data_;
};

}  // namespace proto


#endif  /* SERIALIZED_OBJECT_ */