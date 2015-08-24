#ifndef MESSAGE_H_
#define MESSAGE_H_

namespace proto {

class Message {
 public:
  virtual ~Message() {};

  inline virtual Message* New() = 0;

  virtual void InitAsDefaultInstance() = 0;
};

}  // namespace proto

#endif  /* MESSAGE_H_ */