#pragma once

namespace s2 {

template <typename T>
struct null_iterator {
  T& operator*() { return t; }
  auto operator++() { ++count; return *this; }
  auto operator--() { --count; return *this; }
  auto operator++(int) { ++count; return *this; }
  auto operator--(int) { --count; return *this; }
  int count = 0;
private:
  T t;
};

}

