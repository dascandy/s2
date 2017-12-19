#pragma once

#include "classify.h"

namespace s2 {

template <typename S>
auto substr(const S &s, size_t start, size_t length) {
  auto v = s.view();
  assert(v.size_ > start + length);
  return decltype(v)(v.ptr_ + start, length);
}

template <typename S>
auto trim_left(const S &s) {
  auto v = s.view();
  while (!v.empty() && isspace(v.front())) {
    v.ptr_++;
    v.size_--;
  }
  return v;
}

template <typename S>
auto trim_right(const S &s) {
  auto v = s.view();
  while (!v.empty() && isspace(v.back())) {
    v.size_--;
  }
  return v;
}

template <typename S>
auto trim(const S &s) {
  auto v = s.view();
  while (!v.empty() && isspace(v.front())) {
    v.ptr_++;
    v.size_--;
  }
  while (!v.empty() && isspace(v.back())) {
    v.size_--;
  }
  return v;
}

template <typename S>
bool only_whitespace(const S& s) {
  auto v = s.view();
  for (auto c : v) {
    if (!isspace(c)) return false;
  }
  return true;
}

}


