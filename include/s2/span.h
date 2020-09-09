#pragma once

#include <initializer_list>

namespace s2 {

template <typename T>
struct span {
  constexpr span(std::initializer_list<T> l)
  : data_(l.begin())
  , size_(l.size())
  {
  }
  template <size_t N>
  constexpr span(T (&t)[N])
  : data_(t)
  , size_(N)
  {
  }
  constexpr span(T* p, size_t N)
  : data_(p)
  , size_(N)
  {
  }  
  constexpr T& operator[](size_t offset) { return data_[offset]; }
  constexpr T* data() { return data_; }
  constexpr size_t size() { return size_; }
  T* data_;
  size_t size_;
};
  
}

