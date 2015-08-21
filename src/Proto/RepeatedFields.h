#ifndef PROTO_REPEATED_FIELDS_
#define PROTO_REPEATED_FIELDS_

#include <vector>

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
  ~RepeatedFieldBase() {}

  virtual int size() const = 0;
  virtual void Clear() = 0;
  virtual bool empty() const = 0;
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
  int size() const override;
  const T Get(const int index) const;
  void Set(const int index, const T& value);
  void Clear() override;
  bool empty() const override { return elements.size() == 0; }
  const std::vector<T>& GetElements() const { return elements; }
  std::vector<T>& Mutable_Elements() { return elements; }

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
template <typename T> class RepeatedFieldIterator;

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
  bool empty() const override { return elements.size() == 0; }
  const std::vector<T*>& GetElements() const { return elements; }
  std::vector<T*>& Mutable_Elements() { return elements; }

 private:
  std::vector<T*> elements;
};

// template <typename T>
// class RepeatedFieldIterator :
//     public std::iterator<std::random_access_iterator_tag, T> {
//  public:
//   typedef RepeatedFieldIterator<T> iterator;
//   typedef std::iterator<std::random_access_iterator_tag, T> superclass;

//   typedef typename remove_const<T>::type value_type; // ?

//   typedef typename superclass::reference reference;
//   typedef typename superclass::pointer pointer;
//   typedef typename superclass::difference_type difference_type;

//   explicit RepeatedFieldIterator(const std::vector<T>* v) : v_(v) {}
//   RepeatedFieldIterator(const std::vector<T>* v, const int index) :
//       v_(v),
//       index_(index) {}

//   RepeatedFieldIterator(const RepeatedFieldIterator& other) :
//       v_(other.v_),
//       index_(other.index_) {}

//   reference operator*() const { return (*v_)[index_]; }
//   pointer operator->() const { return &(operator*()); }
//   reference operator[](int index) const { return (*v_)[index]; }

//   iterator& operator=(const iterator& other) {
//     v_ = other.v_;
//     index_ = other.index_;
//     return *this;
//   }

//   bool operator==(const iterator& other) const {
//     return v_ == other.v_ && index_ == other.index_;
//   }

//   bool operator!=(const iterator& other) const {
//     return v_ != other.v_ || index_ != other.index_;
//   }

//   iterator& operator++() { ++index_; return *this; }
//   iterator operator++(int) {
//     iterator temp = iterator(*this);
//     index_++;
//     return temp;
//   }
//   iterator& operator--() { --index_; return *this; }
//   iterator operator--(int) {
//     iterator temp = iterator(*this);
//     index_--;
//     return temp;
//   }

//   iterator& operator+=(difference_type diff) { index_ += diff; return *this; }
//   friend iterator operator+(const difference_type diff, iterator it) {
//     it += diff;
//     return it;
//   }
//   iterator& operator-=(difference_type diff) { index_ -= diff; return *this; }
//   friend iterator operator-(const difference_type diff, iterator it) {
//     it -= diff;
//     return it;
//   }

//  private:
//   const std::vector<T>* v_;
//   int index_ = 0;
//   template <typename OtherT>
//   friend class RepeatedFieldIterator;
// };



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
int RepeatedField<T>::size() const {
  return elements.size();
}

template <typename T>
const T RepeatedField<T>::Get(const int index) const {
  return elements[index];
}

template <typename T>
void RepeatedField<T>::Set(const int index, const T& value) {
  if (index < (int)elements.size()) {
    elements[index] = value;
  }
}

template <typename T>
void RepeatedField<T>::Clear() {
  elements.clear();
}

template <typename T>
inline typename RepeatedField<T>::iterator
RepeatedField<T>::begin() {
  std::cout << "begin called" << std::endl;
  return elements.begin();
}

template <typename T>
inline typename RepeatedField<T>::const_iterator
RepeatedField<T>::begin() const {
  std::cout << "const begin" << std::endl;
  return elements.begin();
}

template <typename T>
inline typename RepeatedField<T>::const_iterator
RepeatedField<T>::cbegin() const {
  std::cout << "cbegin called" << std::endl;
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
void RepeatedPtrField<T>::AddAllocated(const T* value) {
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
int RepeatedPtrField<T>::size() const {
  return elements.size();
}

template <typename T>
const T& RepeatedPtrField<T>::Get(const int index) const {
  return *elements[index];
}

template <typename T>
T* RepeatedPtrField<T>::GetMutable(const int index) {
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


}  // namespace proto

#endif  /* PROTO_REPEATED_FIELDS_ */