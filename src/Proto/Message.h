#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "SerializedMessage.h"

namespace proto {

class Message {
 public:
  virtual ~Message() {};

  virtual Message* New() const = 0;

  virtual void InitAsDefaultInstance() = 0;

  virtual SerializedMessage* Serialize() const = 0;

  virtual void DeSerialize(const char* buf, unsigned int size) = 0;  

  virtual void Print(int indent_num=0) const {}

 protected:
  void PrintIndent(int num) const;
};

}  // namespace proto

#endif  /* MESSAGE_H_ */