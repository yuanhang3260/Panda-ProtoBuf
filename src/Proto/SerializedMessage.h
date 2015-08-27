#ifndef SERIALIZED_OBJECT_
#define SERIALIZED_OBJECT_

#include <vector>
#include <memory>

#include "SerializedObjectInterface.h"

namespace proto {

class SerializedMessage: public SerializedObjectInterface {
 public:
  SerializedMessage() {}
  ~SerializedMessage() {}

  int CopyTo(char* buf) const override;

  void AddField(std::shared_ptr<SerializedObjectInterface> new_field);

 private:
  vector<std::shared_ptr<SerializedObjectInterface>> fields_;
};

}  // namespace proto


#endif  /* SERIALIZED_OBJECT_ */