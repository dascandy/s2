#pragma once

#include <s2/vector>
#include <s2/detail/encoding_utf8.h>
#include <s2/detail/string_view.h>

namespace s2 {

template <typename encoding>
class basic_string
{
public:
  basic_string() { storage_.push_back(0); }
  basic_string(const basic_string<encoding>& sv) : storage_(sv.storage_) {}
  basic_string(const basic_string_view<encoding>& sv) : storage_(sv.data(), sv.data() + sv.size()) {
    storage_.push_back(0);
  }
  template <size_t N>
  basic_string(const typename encoding::char_type (&array)[N])
  : storage_(array, array + N)
  {}
  template <size_t N>
  explicit basic_string(vector<typename encoding::storage_type, N>&& sv) : storage_(std::move(sv)) {
    if (storage_.back() != 0) storage_.push_back(0);
  }
  template <size_t N>
  explicit basic_string(const vector<typename encoding::storage_type, N>& sv) : storage_(sv) {
    if (storage_.back() != 0) storage_.push_back(0);
  }
  template <typename... Ts>
  basic_string& operator=(rope<Ts...>&& r) {
    clear();
    (*this) += std::forward(r);
  }
  template <typename... Ts>
  basic_string& operator+=(rope<Ts...>&& r) {
    size_t strl = get_string_length<encoding>(r.t);
    storage_.reserve(storage_.size() + strl);
    get_string_contents<encoding>(std::back_inserter(storage_), std::move(r.t));
    return *this;
  }
  template <typename... Ts>
  basic_string(rope<Ts...>&& r) {
    size_t strl = get_string_length<encoding>(r.t);
    storage_.reserve(strl + 1);
    get_string_contents<encoding>(std::back_inserter(storage_), std::move(r.t));
  }
  template <typename SEncoding>
  basic_string(const basic_string_view<SEncoding>& sv) {
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
  basic_string_view<encoding> view() { return basic_string_view<encoding>(storage_.data(), storage_.size() - 1); }
  operator basic_string_view<encoding>() { return view(); }
  const s2::vector<typename encoding::storage_type> &storage() const { return storage_; }
private:
  s2::vector<typename encoding::storage_type, 32> storage_;
};

using string = basic_string<s2::encoding::utf8>;

}


