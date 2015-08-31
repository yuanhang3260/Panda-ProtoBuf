#ifndef PROTO_REPEATED_FIELDS_
#define PROTO_REPEATED_FIELDS_

#include <iostream>
#include <vector>
#include <stdexcept>

namespace proto {

template<typename T>
struct remove_const {
  typedef T type;
};

template<typename T>
struct remove_const<T const> {
  typedef T type;
};

class RepeatedFieldBase {
 public:
  RepeatedFieldBase() = default;
  virtual ~RepeatedFieldBase() {}

  virtual unsigned int size() const = 0;
  virtual void Clear() = 0;
  virtual bool empty() const = 0;
  virtual const char* GetElementPtr(const unsigned int index) const = 0;
};

// ----------------------------- RepeatedField ------------------------------ //
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
  unsigned int size() const override;
  const T Get(const int index) const;
  void Set(const int index, const T& value);
  void Clear() override;
  bool empty() const override { return elements.size() == 0; }
  const std::vector<T>& GetElements() const { return elements; }
  std::vector<T>& Mutable_Elements() { return elements; }
  const char* GetElementPtr(const unsigned int index) const override;

  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef T value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef int size_type;
  // typedef ptrdiff_t difference_type;

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;

 private:
  std::vector<T> elements;
};

// -------------------------- RepeatedPtrField ------------------------------ //
template <typename T, bool is_const_iterator> class RepeatedPtrIterator;

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

  void AddAllocated(T* value);
  void Set(int index, const T& value); // Only used for repeated string type.
  T* Add();
  void RemoveLast();
  unsigned int size() const override;
  const T& Get(const int index) const;
  T* GetMutable(const int index);
  void Clear() override;
  bool empty() const override { return elements.size() == 0; }
  const std::vector<T*>& GetElements() const { return elements; }
  std::vector<T*>& Mutable_Elements() { return elements; }
  const char* GetElementPtr(const unsigned int index) const override;

  typedef RepeatedPtrIterator<T, false> iterator;
  typedef RepeatedPtrIterator<T, true> const_iterator;
  typedef T value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef int size_type;
  // typedef ptrdiff_t difference_type;

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;

 private:
  std::vector<T*> elements;
};

// ------------------------ RepeatedPtrIterator ----------------------------- //
template <typename T, bool is_const_iterator = true>
class RepeatedPtrIterator :
    public std::iterator<std::random_access_iterator_tag, T> {
 public:
  typedef RepeatedPtrIterator iterator;
  typedef std::iterator<std::random_access_iterator_tag, T> superclass;

  // typedef typename superclass::reference reference;
  // typedef typename superclass::pointer pointer;
  typedef typename 
  std::conditional<is_const_iterator, const T*, T*>::type pointer;
  typedef typename
  std::conditional<is_const_iterator, const T&, T&>::type reference;
  typedef typename superclass::difference_type difference_type;

  RepeatedPtrIterator() : v_(NULL) {}
  explicit RepeatedPtrIterator(const std::vector<T*>* v) : v_(v) {}
  RepeatedPtrIterator(const std::vector<T*>* v, const int index) :
      v_(v),
      index_(index) {}

  RepeatedPtrIterator(const RepeatedPtrIterator<T, false>& other) :
      v_(other.v_),
      index_(other.index_) {}

  RepeatedPtrIterator& operator=(const RepeatedPtrIterator<T, false>& other) {
    v_ = other.v_;
    index_ = other.index_;
    return *this;
  }

  reference operator*() const {
    return *((*v_)[index_]);
  }

  pointer operator->() const { return &(operator*()); }

  reference operator[](int index) const {
    return *((*v_)[index]);
  }

  bool operator==(const iterator& other) const {
    return v_ == other.v_ && index_ == other.index_;
  }

  bool operator!=(const iterator& other) const {
    return v_ != other.v_ || index_ != other.index_;
  }

  iterator& operator++() { ++index_; return *this; }
  iterator operator++(int) {
    iterator temp = iterator(*this);
    index_++;
    return temp;
  }
  iterator& operator--() { --index_; return *this; }
  iterator operator--(int) {
    iterator temp = iterator(*this);
    index_--;
    return temp;
  }

  iterator& operator+=(difference_type diff) { index_ += diff; return *this; }
  friend iterator operator+(const difference_type diff, iterator it) {
    it += diff;
    return it;
  }
  iterator& operator-=(difference_type diff) { index_ -= diff; return *this; }
  friend iterator operator-(const difference_type diff, iterator it) {
    it -= diff;
    return it;
  }

 private:
  const std::vector<T*>* v_;
  int index_ = 0;
  template <typename OtherT, bool is_const_iterator_other>
  friend class RepeatedPtrIterator;
};

// ===========================================================================//
// =========================== Implementation =============================== //
// ===========================================================================//

// ---------------------------- RepeatedField --------------------------------//
template <typename T>
RepeatedField<T>::RepeatedField() {
}

template <typename T>
void RepeatedField<T>::Add(const T value) {
  elements.push_back(value);
}

template <typename T>
void RepeatedField<T>::RemoveLast() {
  return elements.pop_back();
}

template <typename T>
unsigned int RepeatedField<T>::size() const {
  return elements.size();
}

template <typename T>
const T RepeatedField<T>::Get(const int index) const {
  if (index >= elements.size()) {
    throw std::runtime_error(
        "RepeatedField index " + std::to_string(index) + " out of bound");
  }
  return elements[index];
}

template <typename T>
const char* RepeatedField<T>::GetElementPtr(const unsigned int index) const {
  if (index < elements.size()) {
    return reinterpret_cast<const char*>(&(elements[index]));
  }
  return nullptr;
}

template <typename T>
void RepeatedField<T>::Set(const int index, const T& value) {
  if (index < (int)elements.size()) {
    elements[index] = value;
  }
  else {
    throw std::runtime_error(
        "RepeatedField index " + std::to_string(index) + " out of bound");
  }
}

template <typename T>
void RepeatedField<T>::Clear() {
  elements.clear();
}

template <typename T>
inline typename RepeatedField<T>::iterator
RepeatedField<T>::begin() {
  // std::cout << "begin called" << std::endl;
  return elements.begin();
}

template <typename T>
inline typename RepeatedField<T>::const_iterator
RepeatedField<T>::begin() const {
  // std::cout << "const begin" << std::endl;
  return elements.begin();
}

template <typename T>
inline typename RepeatedField<T>::const_iterator
RepeatedField<T>::cbegin() const {
  // std::cout << "cbegin called" << std::endl;
  return elements.begin();
}

template <typename T>
inline typename RepeatedField<T>::iterator
RepeatedField<T>::end() {
  return elements.end();
}

template <typename T>
inline typename RepeatedField<T>::const_iterator
RepeatedField<T>::end() const {
  return elements.end();
}

template <typename T>
inline typename RepeatedField<T>::const_iterator
RepeatedField<T>::cend() const {
  return elements.end();
}

// -------------------------- RepeatedPtrField -------------------------------//
template <typename T>
RepeatedPtrField<T>::RepeatedPtrField() {
}

template <typename T>
void RepeatedPtrField<T>::AddAllocated(T* value) {
  if (value) {
    elements.push_back(value);
  }
}

template <typename T>
void RepeatedPtrField<T>::Set(const int index, const T& value) {
  if (index < (int)elements.size()) {
    if (elements[index]) {
      *elements[index] = value;
    }
    else {
      elements[index] = new T(value);
    }
  }
  else {
    throw std::runtime_error(
        "RepeatedPtrField index " + std::to_string(index) + " out of bound");
  }
}

template <typename T>
T* RepeatedPtrField<T>::Add() {
  T* new_obj = new T();
  elements.push_back(new_obj);
  return new_obj;
}

template <typename T>
void RepeatedPtrField<T>::RemoveLast() {
  if (elements[elements.size()-1]) {
    delete elements[elements.size()-1];
  }
  return elements.pop_back();
}

template <typename T>
unsigned int RepeatedPtrField<T>::size() const {
  return elements.size();
}

template <typename T>
const T& RepeatedPtrField<T>::Get(const int index) const {
  if (index >= elements.size()) {
    throw std::runtime_error(
        "RepeatedPtrField index " + std::to_string(index) + " out of bound");
  }
  return *elements[index];
}

template <typename T>
const char* RepeatedPtrField<T>::GetElementPtr(const unsigned int index) const {
  if (index < elements.size()) {
    return reinterpret_cast<const char*>(elements[index]);
  }
  return nullptr;
}

template <typename T>
T* RepeatedPtrField<T>::GetMutable(const int index) {
  if (index >= elements.size()) {
    throw std::runtime_error(
        "RepeatedPtrField index " + std::to_string(index) + " out of bound");
  }
  return elements[index];
}

template <typename T>
void RepeatedPtrField<T>::Clear() {
  for (T* p: elements) {
    if (p) {
      delete p;
    }
  }
  elements.clear();
}

template <typename T>
inline typename RepeatedPtrField<T>::iterator
RepeatedPtrField<T>::begin() {
  std::cout << "begin called" << std::endl;
  return iterator(&elements);
}

template <typename T>
inline typename RepeatedPtrField<T>::const_iterator
RepeatedPtrField<T>::begin() const {
  std::cout << "const begin" << std::endl;
  return const_iterator(&elements);
}

template <typename T>
inline typename RepeatedPtrField<T>::const_iterator
RepeatedPtrField<T>::cbegin() const {
  // std::cout << "cbegin called" << std::endl;
  return const_iterator(&elements);
}

template <typename T>
inline typename RepeatedPtrField<T>::iterator
RepeatedPtrField<T>::end() {
  return iterator(&elements, elements.size());
}

template <typename T>
inline typename RepeatedPtrField<T>::const_iterator
RepeatedPtrField<T>::end() const {
  return const_iterator(&elements, elements.size());
}

template <typename T>
inline typename RepeatedPtrField<T>::const_iterator
RepeatedPtrField<T>::cend() const {
  return const_iterator(&elements, elements.size());
}


}  // namespace proto

#endif  /* PROTO_REPEATED_FIELDS_ */