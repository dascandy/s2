#pragma once

namespace s2 {

template <typename T>
struct null_iterator {
  T& operator*() { return t; }
  auto operator++() { return *this; }
  auto operator--() { return *this; }
  auto operator++(int) { return *this; }
  auto operator--(int) { return *this; }
private:
  T t;
};

}

