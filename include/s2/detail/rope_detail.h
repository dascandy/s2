#pragma once

namespace s2 {

template <typename encoding, typename T>
size_t get_string_length1(const T& t) {
  // return size of T as string's content in encoding
}
template <typename encoding, typename It, typename T>
void get_string_content(It& it, const T& t) {
  
}

template <typename encoding>
size_t get_string_length1(const string_view<encoding>& t) {
  return t.storage().size() - 1;
}
template <typename encoding, typename It>
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
template <typename encoding, typename It, typename encoding2>
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
template <typename encoding, typename It>
void get_string_content(It& it, uint64_t n) {
  uint64_t n2 = n/10;
  if (n2) get_string_content(it, n2);
  *it++ = '0' + n % 10;
}

template <typename encoding, typename It, typename... Ts, int... inds>
size_t get_string_length_(It& it, std::tuple<Ts...> ts, std::index_sequence<inds...>) {
  return (get_string_length1(it, std::get<inds>(ts)) + ... + 0);
}
template <typename encoding, typename It, typename... Ts>
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

}


