#pragma once

#include <type_traits>
#include <initializer_list>
#include <assert.h>
#include <new>

namespace s2 {

template<class T, size_t maxsize>
typename vector<T, maxsize>::iterator vector<T, maxsize>::begin() {
  return iterator(ptr());
}
template<class T, size_t maxsize>
typename vector<T, maxsize>::iterator vector<T, maxsize>::end() {
  return iterator(ptr() + size());
}

template<class T, size_t maxsize>
typename vector<T, maxsize>::const_iterator vector<T, maxsize>::begin() const {
  return const_iterator(ptr());
}
template<class T, size_t maxsize>
typename vector<T, maxsize>::const_iterator vector<T, maxsize>::end() const {
  return const_iterator(ptr() + size());
}

template<class T, size_t maxsize>
bool vector<T, maxsize>::isExternal() const {
  return innerSize_ == 255; 
}

template<class T, size_t maxsize>
T* vector<T, maxsize>::ptr() {
  return isExternal() ? reinterpret_cast<inner*>(&storage)->ptr_ : reinterpret_cast<T*>(&storage);
}

template<class T, size_t maxsize>
const T* vector<T, maxsize>::ptr() const {
  return isExternal() ? reinterpret_cast<const inner*>(&storage)->ptr_ : reinterpret_cast<const T*>(&storage);
}

template<class T, size_t maxsize>
size_t vector<T, maxsize>::capacity() const {
  return isExternal() ? reinterpret_cast<const inner*>(&storage)->capacity_ : innerCapacity_;
}

template<class T, size_t maxsize>
size_t vector<T, maxsize>::size() const {
  return isExternal() ? reinterpret_cast<const inner*>(&storage)->size_ : innerSize_;
}

template<class T, size_t maxsize>
void vector<T, maxsize>::reserve(size_t newCapacity) {
  if (newCapacity <= capacity()) return;
  // TODO: alignment // TODO: exception safety
  T* newPtr = reinterpret_cast<T*>(new unsigned char[sizeof(T) * newCapacity]);
  size_t tsize = size();
  for (size_t n = 0; n < tsize; n++) {
    new (&newPtr[n])T(std::move(ptr()[n]));
    ptr()[n].~T();
  }
  if (isExternal())
    delete [] (reinterpret_cast<unsigned char*>(reinterpret_cast<inner*>(&storage)->ptr_));
  innerSize_ = 255;
  inner* in = reinterpret_cast<inner*>(&storage);
  in->ptr_ = newPtr;
  in->capacity_ = newCapacity;
  in->size_ = tsize;
}

template<class T, size_t maxsize>
void vector<T, maxsize>::grow() {
  // holder for grow policy
  if (capacity() == size())
    reserve(2 + capacity() + capacity() / 2);
}

template<class T, size_t maxsize>
void vector<T, maxsize>::shrink_to_fit() {
  // We can't shrink the internal buffer
  if (!isExternal()) return;
  inner* in = reinterpret_cast<inner*>(&storage);
  if (size() < innerCapacity_) {
    // Internalize again
    size_t tsize = size();
    T* tptr = ptr();
    innerSize_ = tsize;
    for (size_t n = 0; n < tsize; n++) {
      new (&ptr()[n])T(std::move(tptr[n]));
      tptr[n].~T();
    }
    delete [] (reinterpret_cast<unsigned char*>(tptr));
  } else if (in->size_ < 3*(in->capacity_/4)) {
    // realloc to smaller buffer, but only if there's a 25% or more gain to be had
    T* tptr = ptr();
    T* newPtr = reinterpret_cast<T*>(new unsigned char[sizeof(T) * size()]);
    for (size_t n = 0; n < size(); n++) {
      new (&newPtr[n])T(std::move(tptr[n]));
      tptr[n].~T();
    }
    delete [] (reinterpret_cast<unsigned char*>(tptr));
    in->ptr_ = newPtr;
    in->capacity_ = size();
  }
}

template<class T, size_t maxsize>
void vector<T, maxsize>::push_back(const T& value) {
  grow();
  new (&ptr()[size()])T(value);
  if (isExternal()) reinterpret_cast<inner*>(&storage)->size_++; else innerSize_++;
}

template<class T, size_t maxsize>
void vector<T, maxsize>::push_back(T&& value) {
  grow();
  new (&ptr()[size()])T(std::forward<decltype(value)>(value));
  if (isExternal()) reinterpret_cast<inner*>(&storage)->size_++; else innerSize_++;
}

template<class T, size_t maxsize>
T vector<T, maxsize>::pop_back() {
  assert(!empty());
  struct do_at_function_exit {
    do_at_function_exit(vector<T>* v) : v(v) {}
    ~do_at_function_exit() {
      if (disable) return;
      v->ptr()[v->size() - 1].~T();
      if (v->isExternal()) reinterpret_cast<inner*>(&v->storage)->size_--; else v->innerSize_--;
    }
    vector<T>* v;
    bool disable = false;
  } _inst(this);
  try {
    return std::move(ptr()[size() - 1]);
  } catch (...) {
    _inst.disable = true;
    throw;
  }
}

// Functions that just rely on the functions above to do what they do

template<class T, size_t maxsize>
constexpr vector<T, maxsize>::vector() noexcept {}

template<class T, size_t maxsize>
vector<T, maxsize>::vector(size_t count) {
  while (count--) push_back(T());
}

template<class T, size_t maxsize>
vector<T, maxsize>::vector(size_t count, const T& value) {
  while (count--) push_back(value);
}

template <class T, size_t maxsize>
vector<T, maxsize>::vector(vector<T, maxsize>&& other) noexcept(std::is_nothrow_move_constructible<T>::value) {
  if (other.isExternal()) {
    innerSize_ = 255;
    inner* in = reinterpret_cast<inner*>(&storage);
    in->size_ = other.size();
    in->capacity_ = other.capacity();
    in->ptr_ = other.ptr();
  } else {
    for (size_t n = 0; n < other.size(); n++) {
      push_back(std::move(other[n]));
    }
  }
}

template<class T, size_t maxsize>
template <typename T2>
vector<T, maxsize>::vector(T2&& ) noexcept(std::is_nothrow_move_constructible<T>::value) {
  // TODO
}

template<class T, size_t maxsize>
vector<T, maxsize>::vector(const vector<T>& other) {
  reserve(other.size());
  assign(other);
}

template<class T, size_t maxsize>
vector<T, maxsize>::vector(std::initializer_list<T> init) {
  assign(init);
}

template<class T, size_t maxsize>
vector<T, maxsize>::~vector() {
  clear();
  shrink_to_fit();
}

template<class T, size_t maxsize>
void vector<T, maxsize>::assign(size_t count, const T& value) {
  clear();
  for (size_t n = 0; n < count; n++) push_back(value);
}

template<class T, size_t maxsize>
void vector<T, maxsize>::assign(std::initializer_list<T> ilist) {
  clear();
  reserve(ilist.size());
  auto it = ilist.begin(), end = ilist.end();
  while (it != end) push_back(*it++);
}

template <class T, size_t maxsize>
void vector<T, maxsize>::resize(size_t newSize) {
  if (newSize < size()) truncate(newSize);
  else {
    reserve(newSize);
    while (size() < newSize)
      push_back({});
  }
}

template<class T, size_t maxsize>
void vector<T, maxsize>::truncate(size_t newSize) {
  for (size_t n = size(); n --> newSize;) {
    ptr()[n].~T();
  }

  if (isExternal()) 
    reinterpret_cast<inner*>(&storage)->size_ = newSize; 
  else 
    innerSize_ = newSize;
}

template<class T, size_t maxsize>
T* vector<T, maxsize>::data() {
  return ptr();
}

template<class T, size_t maxsize>
const T* vector<T, maxsize>::data() const {
  return ptr();
}

template<class T, size_t maxsize>
const T& vector<T, maxsize>::operator[](size_t index) const noexcept {
  assert(index < size());
  return ptr()[index];
}

template<class T, size_t maxsize>
T& vector<T, maxsize>::operator[](size_t index) noexcept {
  assert(index < size());
  return ptr()[index];
}

template<class T, size_t maxsize>
T& vector<T, maxsize>::front() { 
  assert(!empty());
  return ptr()[0];
}

template<class T, size_t maxsize>
T& vector<T, maxsize>::back() { 
  assert(!empty());
  return ptr()[size() - 1]; 
}

template<class T, size_t maxsize>
void vector<T, maxsize>::clear() noexcept {
  truncate(0);
}

template<class T, size_t maxsize>
bool vector<T, maxsize>::empty() noexcept {
  return size() == 0; 
}

template<class T, size_t maxsize>
template<typename ...Args> 
void vector<T, maxsize>::emplace_back(Args&&... args) {
  grow();
  new(&ptr()[size()])T(std::forward<Args>(args)...);
  if (isExternal()) reinterpret_cast<inner*>(&storage)->size_++; else innerSize_++;
}

}



