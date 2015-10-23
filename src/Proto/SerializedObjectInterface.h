#ifndef SERIALIZED_OJBECT_INTERFACE
#define SERIALIZED_OJBECT_INTERFACE

#include <iostream>

class SerializedObjectInterface {
 public:
  SerializedObjectInterface() {}
  virtual ~SerializedObjectInterface() {
    //std::cout << "deleting SerializedObjectInterface" << std::endl;
    if (flat_buf_) {
      delete flat_buf_;
    }
  }

  const char* GetBytes() {
    //std::cout << "GetByte() Size = " << size_ << std::endl;
    if (size_ > 0) {
      flat_buf_ = new char[size_];
      CopyTo(flat_buf_);
      return flat_buf_;
    }
    else {
      return nullptr;
    }
  }

  virtual int CopyTo(char* buf) const = 0;

  virtual unsigned int size() const { return size_; }
  void set_size(const int size) { size_ = size; }

 protected:
  int size_ = 0;
  char* flat_buf_ = nullptr;
};

#endif  /* SERIALIZED_OJBECT_INTERFACE */