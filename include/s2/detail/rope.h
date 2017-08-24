#pragma once

namespace s2 {

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

template <typename encoding, typename T>
class convert {
  size_t length(const T& t) {
    return 1;
  }
  template <typename It>
  void encode(It& it, const T& t) {
    *it++ = 'a';
  }
};

template <typename encoding, typename... Ts, int... inds>
size_t get_string_length_(std::tuple<Ts...> ts, std::index_sequence<inds...>) {
  return (convert<encoding, Ts>::length(std::get<inds>(ts)) + ... + 0);
}
template <typename It, typename encoding, typename... Ts, int... inds>
void get_string_contents_(It& it, std::tuple<Ts...> ts, std::index_sequence<inds...>) {
  (convert<encoding, Ts>::encode(it, std::get<inds>(ts)), ..., 0);
}
template <typename encoding, typename... Ts>
size_t get_string_length(std::tuple<Ts...> t) {
  get_string_length_(t, std::make_index_sequence<sizeof...(Ts)>());
}
template <typename It, typename encoding, typename... Ts>
void get_string_contents(It& it, std::tuple<Ts...> t) {
  get_string_contents_(it, t, std::make_index_sequence<sizeof...(Ts)>());
}

}


