#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Proto/Descriptor.h"
#include "Proto/SerializedMessage.h"

namespace proto {

class MessageReflection;

class Message {
 public:
  virtual ~Message() {};

  virtual Message* New() const = 0;

  virtual void InitAsDefaultInstance() = 0;

  virtual SerializedMessage* Serialize() const = 0;

  virtual void DeSerialize(const char* buf, unsigned int size) = 0;  

  virtual void Print(int indent_num=0) const {}

  virtual const MessageDescriptor* GetDescriptor() const = 0;

  virtual const MessageReflection* GetReflection() const = 0;

 protected:
  void PrintIndent(int num) const;
};

}  // namespace proto

#endif  /* MESSAGE_H_ */