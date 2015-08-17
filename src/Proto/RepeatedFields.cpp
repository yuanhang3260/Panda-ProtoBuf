#include "RepeatedFields.h"

namespace proto {

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
