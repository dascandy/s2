#pragma once

#include <type_traits>
#include <initializer_list>
#include <assert.h>
#include <new>
#include <cstdint>

namespace s2 {

constexpr size_t max(size_t a, size_t b) { return (a > b) ? a : b; }

template<class T, size_t maxsize = 128>
class alignas(max(alignof(T), alignof(T*))) vector
{
  // properly aligned uninitialized storage for N T's
  static constexpr size_t innerCapacity_ = (maxsize - 2) / sizeof(T);
  struct inner {
    std::size_t size_;
    std::size_t capacity_;
    T* ptr_;
  };
  std::aligned_storage_t<max(innerCapacity_ * sizeof(T), sizeof(inner)), 1> storage;
  uint8_t innerSize_ = 0;
  static_assert(innerCapacity_ < 255, "Cannot make a local storage of larger than 255 local items");
  bool isExternal() const;
  T* ptr();
  const T* ptr() const;
public:
  using value_type = T;
  struct iterator;
  struct const_iterator;
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  size_t capacity() const;
  size_t size() const;
  void reserve(size_t newCapacity);
  void resize(size_t size);
  void grow();
  void shrink_to_fit();
  constexpr vector() noexcept;
  explicit vector(size_t count);
  template <typename IT>
  vector(IT begin, IT end);
  vector(size_t count, const T& value);
  vector(const vector<T, maxsize>& other) noexcept(std::is_nothrow_move_constructible<T>::value);
  template <typename T2>
  vector(T2&& other) noexcept(std::is_nothrow_move_constructible<T>::value);
  template <size_t N>
  vector(const T (&arr)[N]) noexcept(std::is_nothrow_move_constructible<T>::value);
  vector(std::initializer_list<T> init);
  ~vector();
  void assign(size_t count, const T& value);
  void assign(std::initializer_list<T> ilist);
  template <typename R>
  void assign(const R& range);
  template <typename R>
  void append(const R& range);
  void truncate(size_t newSize);
  T* data();
  const T* data() const;
  const T& operator[](size_t index) const noexcept;
  bool operator==(const vector<T, maxsize>& rhs) const;
  T& operator[](size_t index) noexcept;
  const T& at(size_t index) const;
  T& at(size_t index);
  T& front();
  T& back();
  void push_back(const T& value);
  void push_back(T&& value);
  T pop_back();
  void clear() noexcept;
  bool empty() noexcept;
  template<typename ...Args> 
  void emplace_back(Args&&... args);
};

template<class T, size_t maxsize>
struct vector<T, maxsize>::iterator {
  iterator(T* ptr) : ptr(ptr) {}
  bool operator==(const iterator& it) const { return ptr == it.ptr; }
  bool operator!=(const iterator& it) const { return !(*this == it); }
  iterator& operator++() { ptr++; return *this; }
  iterator operator++(int) { iterator copy = *this; ptr++; return copy; }
  iterator& operator--() { ptr--; return *this; }
  iterator operator--(int) { iterator copy = *this; ptr--; return copy; }
  T& operator*() { return *ptr; }
  T* operator->() { return ptr; }
private:
  T* ptr;
};

template<class T, size_t maxsize>
struct vector<T, maxsize>::const_iterator {
  const_iterator(const T* ptr) : ptr(ptr) {}
  bool operator==(const const_iterator& it) const { return ptr == it.ptr; }
  bool operator!=(const const_iterator& it) const { return !(*this == it); }
  const_iterator& operator++() { ptr++; return *this; }
  const_iterator operator++(int) { const_iterator copy = *this; ptr++; return copy; }
  const_iterator& operator--() { ptr--; return *this; }
  const_iterator operator--(int) { const_iterator copy = *this; ptr--; return copy; }
  const T& operator*() { return *ptr; }
  const T* operator->() { return ptr; }
private:
  const T* ptr;
};

}

#include "detail/vector.h"


