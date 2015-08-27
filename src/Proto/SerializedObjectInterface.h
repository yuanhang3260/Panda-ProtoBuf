#ifndef SERIALIZED_OJBECT_INTERFACE
#define SERIALIZED_OJBECT_INTERFACE

class SerializedObjectInterface {
 public:
  SerializedObjectInterface() {}
  ~SerializedObjectInterface() {
    if (flat_buf_) {
      delete flat_buf_;
    }
  }

  const char* GetBytes() {
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

  virtual int size() const { return size_; }

 protected:
  int size_ = 0;
  char* flat_buf_ = nullptr;
};

#endif  /* SERIALIZED_OJBECT_INTERFACE */