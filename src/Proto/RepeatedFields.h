#ifndef PROTO_REPEATED_FIELDS_
#define PROTO_REPEATED_FIELDS_

#include <vector>

namespace proto {

class RepeatedFieldBase {
 public:
  RepeatedFieldBase() = default;
  ~RepeatedFieldBase() {}

  virtual int size() const = 0;
  virtual void Clear() = 0;
};

template <typename T>
class RepeatedField: public RepeatedFieldBase {
 public:
  RepeatedField<T>();
  RepeatedField<T>(const RepeatedField<T>& other) = default;
  RepeatedField<T>& operator=(const RepeatedField<T>&) = default;
  RepeatedField<T>(RepeatedField<T>&& other) {
    elements = std::move(other.Mutable_Elements());
  };
  RepeatedField<T>& operator=(RepeatedField<T>& other) {
    elements = std::move(other.Mutable_Elements());
    return *this;
  }

  void Add(const T value);
  void RemoveLast();
  int size() const override;
  const T Get(const int index) const;
  void Set(const int index, const T& value);
  void Clear() override;
  const std::vector<T>& GetElements() const { return elements; }
  std::vector<T>& Mutable_Elements() { return elements; }

 private:
  std::vector<T> elements;
};

template <typename T>
class RepeatedPtrField: public RepeatedFieldBase {
 public:
  RepeatedPtrField<T>();
  ~RepeatedPtrField<T>() { Clear(); }
  // disallow copy because we have pointer vector.
  RepeatedPtrField<T>(const RepeatedPtrField<T>&) = delete;
  RepeatedPtrField<T>& operator=(const RepeatedPtrField<T>&) = delete;
  RepeatedPtrField<T>(RepeatedPtrField<T>&& other) {
    elements = std::move(other.Mutable_Elements());
  };
  RepeatedPtrField<T>& operator=(RepeatedPtrField<T>&& other) {
    elements = std::move(other.Mutable_Elements());
    return *this;
  }

  void AddAllocated(const T* value);
  void Set(int index, const T& value); // Only used for repeated string type.
  T* Add();
  void RemoveLast();
  int size() const override;
  const T& Get(const int index) const;
  T* GetMutable(const int index);
  void Clear() override;
  const std::vector<T*>& GetElements() const { return elements; }
  std::vector<T*>& Mutable_Elements() { return elements; }

 private:
  std::vector<T*> elements;
};


}  // namespace proto

#endif  /* PROTO_REPEATED_FIELDS_ */