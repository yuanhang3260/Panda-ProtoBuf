#ifndef SERIALIZED_OBJECT_
#define SERIALIZED_OBJECT_

#include <vector>
#include <memory>

#include "SerializedObjectInterface.h"

namespace proto {

class SerializedMessage: public SerializedObjectInterface {
 public:
  SerializedMessage() {}
  SerializedMessage(const SerializedMessage& other) = delete;
  SerializedMessage& operator=(const SerializedMessage& other) = delete;
  ~SerializedMessage() {}

  int CopyTo(char* buf) const override;

  void AddField(std::shared_ptr<SerializedObjectInterface> new_field);

 private:
  std::vector<std::shared_ptr<SerializedObjectInterface>> fields_;
  int tagWord_;
};

}  // namespace proto


#endif  /* SERIALIZED_OBJECT_ */