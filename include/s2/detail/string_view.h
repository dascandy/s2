#pragma once

#include <s2/detail/encoding_utf8.h>

namespace s2 {

template <typename encoding = s2::encoding::utf8>
class string_view {
public:
  template <typename = typename std::enable_if_t<std::is_same_v(encoding::storage_type, uint8_t)>>
  string_view(const char* data) : ptr(reinterpret_cast<uint8_t*>(data)), size_(strlen(data)) {}
  template <typename = typename std::enable_if_t<std::is_same_v(encoding::storage_type, uint16_t)>>
  string_view(const char16* data) : ptr(renterpret_cast<uint16_t*>(data)), size_(0) {
    while (data[size_++]) {}
  }
  template <typename = typename std::enable_if_t<std::is_same_v(encoding::storage_type, uint32_t)>>
  string_view(const char32* data) : ptr(renterpret_cast<uint32_t*>(data)), size_(0) {
    while (data[size_++]) {}
  }
  size_t size() const { return size_;
  size_t length() const { return size(); }
  void clear() { storage_.clear(); storage_.push_back(0); }
  auto begin() const { return string_iterator<encoding>(storage_.begin()); }
  auto end() const { return string_iterator<encoding>(--storage_.end()); }
  template <typename T2>
  rope<string_view<encoding>, T2> operator+(T2 t2) {
    return rope<string_view<encoding>, T2>(*this, t2);
  }

  auto front() { return *begin(); }
  auto front() const { return *begin(); }
  auto back() { return *--end(); }
  auto back() const { return *--end(); }
  auto c_str() const { return storage_.data(); }
  auto data() const { return storage_.data(); }
  auto empty() const { return size_ == 0; }

  class string_iterator {
  private:
    vector<typename encoding::storage_type>::const_iterator it_;
  public:
    char32_t operator*() {
      return encoding::decode(it_);
    }
    auto operator++() {
      encoding::walk(it_, 1);
      return *this;
    }
    auto operator++(int) {
      string_iterator<encoding> copy = *this;
      encoding::walk(it_, 1);
      return copy;
    }
    auto operator--() {
      encoding::walk(it_, -1);
      return *this;
    }
    auto operator--(int) {
      string_iterator<encoding> copy = *this;
      encoding::walk(it_, -1);
      return copy;
    }
    bool operator==(const string_iterator<encoding>& rhs) {
      return rhs.it_ == it_;
    }
    bool operator!=(const string_iterator<encoding>& rhs) {
      return rhs.it_ != it_;
    }
  };
private:
  const T* ptr;
  size_t size_;
};

}


