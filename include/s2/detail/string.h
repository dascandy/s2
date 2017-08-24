#pragma once

#include <s2/vector>
#include <s2/detail/encoding_utf8.h>

namespace s2 {

template <typename encoding = s2::encoding::utf8>
class string
{
public:
  string() : storage_(1) {}
  string(const string<encoding>& sv) : storage_(sv.storage_) {}
  string(const string_view<encoding>& sv) : storage_(sv.data(), sv.data() + sv.size()) {
    storage_.push_back(0);
  }
  template <size_t N>
  string(vector<typename encoding::storage_type, N>&& sv) : storage_(std::forward(sv)) {
    if (storage_.back() != 0) storage_.push_back(0);
  }
  template <size_t N>
  string(const vector<typename encoding::storage_type, N>& sv) : storage_(sv) {
    if (storage_.back() != 0) storage_.push_back(0);
  }
  template <typename... Ts>
  string& operator=(rope<Ts...>&& r) {
    clear();
    (*this) += std::forward(r);
  }
  template <typename... Ts>
  string& operator+=(rope<Ts...>&& r) {
    size_t strl = get_string_length<encoding>(r);
    storage_.reserve(storage.size() + strl + 1);
    get_string_contents<encoding>(std::back_inserter(storage_), std::forward(r));
    return *this;
  }
  template <typename... Ts>
  string(rope<Ts...>&& r) {
    size_t strl = get_string_length<encoding>(r);
    storage_.reserve(strl + 1);
    get_string_contents<encoding>(std::back_inserter(storage_), std::forward(r));
  }
  template <typename SEncoding>
  string(const string_view<SEncoding>& sv) {
    null_iterator<typename encoding::storage_type> n;
    size_t unitcount = 1;
    for (auto c : sv) {
      unitcount += encoding::encode(n, c);
    }
    storage_.reserve(unitcount);
    auto ins = std::back_inserter(storage_);
    for (auto c : sv) {
      encoding::encode(ins, c);
    }
    storage_.push_back(0);
  }
  void clear() {
    storage_.clear();
    storage_.push_back(0);
  }
  const typename encoding::storage_type* data() const { return storage_.data(); }
  const typename encoding::storage_type* c_str() const { return storage_.data(); }
  operator string_view<encoding>() { return string_view<encoding>(storage_.data(), storage_.size() - 1); }
  const std::vector<typename encoding::storage_type> &storage() const { return storage_; }
private:
  vector<typename encoding::storage_type, 32> storage_;
};

}


