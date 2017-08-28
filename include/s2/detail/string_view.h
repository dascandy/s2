#pragma once

#include <s2/detail/encoding_utf8.h>

namespace s2 {

template <typename encoding>
class basic_string_view {
public:
  static constexpr size_t string_length(const typename encoding::char_type* data) {
    size_t size = 0;
    while (data[size]) size++;
    return size;
  }
  basic_string_view(const typename encoding::storage_type* data, const size_t size) 
  : ptr_(data)
  , size_(size)
  {}
  basic_string_view(const typename encoding::char_type* data, const size_t size) 
  : ptr_(reinterpret_cast<const typename encoding::storage_type*>(data))
  , size_(size)
  {}
  basic_string_view(const typename encoding::char_type* data) : basic_string_view(data, string_length(data)) {}
  auto begin() const { return string_iterator(ptr_); }
  auto end() const { return string_iterator(ptr_ + size_); }
  template <typename T2>
  rope<basic_string_view<encoding>, T2> operator+(T2 t2) {
    return rope<basic_string_view<encoding>, T2>(std::tuple<basic_string_view<encoding>, T2>(*this, t2));
  }

  auto front() { return *begin(); }
  auto front() const { return *begin(); }
  auto back() { return *--end(); }
  auto back() const { return *--end(); }
  auto c_str() const { return ptr_; }
  auto data() const { return ptr_; }
  auto empty() const { return size_ == 0; }

  class string_iterator {
  private:
    const typename encoding::storage_type* it_;
  public:
    string_iterator(const typename encoding::storage_type* it) : it_(it) {}
    char32_t operator*() {
      return encoding::decode(it_);
    }
    auto operator++() {
      encoding::walk(it_, 1);
      return *this;
    }
    auto operator++(int) {
      string_iterator copy = *this;
      encoding::walk(it_, 1);
      return copy;
    }
    auto operator--() {
      encoding::walk(it_, -1);
      return *this;
    }
    auto operator--(int) {
      string_iterator copy = *this;
      encoding::walk(it_, -1);
      return copy;
    }
    bool operator==(const string_iterator& rhs) {
      return rhs.it_ == it_;
    }
    bool operator!=(const string_iterator& rhs) {
      return rhs.it_ != it_;
    }
  };
//private:
  const typename encoding::storage_type* ptr_;
  size_t size_;
/*
  template <typename E>
  friend bool operator==(const basic_string_view<E>& l, const basic_string_view<E>& r);
*/
  template <typename E1, typename E2>
  friend bool operator==(const basic_string_view<E1>& l, const basic_string_view<E2>& r);
};

// Specialization for equal encoding, because then we skip the decoding overhead.
// TODO: check if there is no encoding with overlong encoding allowance, because that breaks this.
/*
template <typename E>
bool operator==(const basic_string_view<E>& l, const basic_string_view<E>& r) {
  if (l.size_ != r.size_) return false;
  auto lc = l.ptr_, rc = r.ptr_;
  auto lend = l.ptr_ + l.size_, rend = r.ptr_ + r.size_;
  while (lc != lend && *lc == *rc) { ++lc; ++rc; }
  return (lc == lend);
}
*/
template <typename E1, typename E2>
bool operator==(const basic_string_view<E1>& l, const basic_string_view<E2>& r) {
  auto lc = l.begin();
  auto lend = l.end();
  auto rc = r.begin();
  auto rend = r.end();
  while (lc != lend && rc != rend && *lc == *rc) { ++lc; ++rc; }
  return (lc == lend && rc == rend);
}

using string_view = basic_string_view<s2::encoding::utf8>;

}


