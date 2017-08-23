#pragma once

namespace s2 {

template <typename encoding, typename T>
size_t get_string_length1(const T& t) {
  // return size of T as string's content in encoding
}
template <typename It, typename encoding, typename T>
void get_string_content(It& it, const T& t) {
  
}

template <typename encoding>
size_t get_string_length1(const string_view<encoding>& t) {
  return t.storage().size() - 1;
}
template <typename encoding>
void get_string_content(It& it, const string_view<encoding>& t) {
  for (auto& s : t.storage()) {
    *it++ = s;
  }
}
template <typename encoding, typename encoding2>
size_t get_string_length1(const string_view<encoding2>& t) {
  size_t len = 0;
  for (auto c : t) {
    len += encoding::encode(null_iterator<uint8_t>(), c);
  }
  return len;
}
template <typename encoding, typename encoding2>
void get_string_content(It& it, const string_view<encoding2>& t) {
  for (auto& s : t) {
    encoding::encode(it, s);
  }
}
template <typename encoding>
size_t get_string_length1(uint64_t n) {
  size_t len = 0;
  while (n > 0) { n /= 10; len++; }
  return len;
}
template <typename encoding>
void get_string_content(It& it, uint64_t n) {
  uint64_t n2 = n/10;
  if (n2) get_string_content(it, n2);
  *it++ = '0' + n % 10;
}

template <typename encoding, typename... Ts, int... inds>
size_t get_string_length_(It& it, std::tuple<Ts...> ts, std::index_sequence<inds...>) {
  return (get_string_length1(it, std::get<inds>(ts)) + ... + 0);
}
template <typename encoding, typename... Ts>
size_t get_string_length(It& it, std::tuple<Ts...> t) {
  get_string_length_(it, t, std::make_index_sequence(sizeof(Ts...)));
}
template <typename It, typename encoding, typename... Ts, int... inds>
void get_string_contents_(It& it, std::tuple<Ts...> ts, std::index_sequence<inds...>) {
  (get_string_content(it, std::get<inds>(ts)), ...);
}
template <typename It, typename encoding, typename... Ts>
void get_string_contents(It& it, std::tuple<Ts...> t) {
  get_string_contents_(it, t, std::make_index_sequence(sizeof(Ts...)));
}

template <typename... Ts>
class rope {
public:
  rope(std::tuple<Ts...> t) : t(t) {}
  template <typename T2>
  rope<Ts..., T2> operator+(T2&& t2) && {
    return rope<Ts..., T2>(std::tuple_cat(std::move(t), std::tuple<T2>(std::forward<T2>(t2))));
  }
private:
  std::tuple<Ts...> t;
};

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
    storage_.resize(storage.size() + strl + 1);
    get_string_contents<encoding>(std::back_inserter(storage_), std::forward(r));
    return *this;
  }
  template <typename... Ts>
  string(rope<Ts...>&& r) {
    size_t strl = get_string_length<encoding>(r);
    storage_.resize(strl + 1);
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
    storage_.resize(1);
    storage_[0] = 0;
  }
  const typename encoding::storage_type* data() const { return storage_.data(); }
  const typename encoding::storage_type* c_str() const { return storage_.data(); }
  operator string_view<encoding>() { return string_view<encoding>(storage_.data(), storage_.size() - 1); }
  const std::vector<typename encoding::storage_type> &storage() const { return storage_; }
private:
  vector<typename encoding::storage_type, 32> storage_;
};

}


