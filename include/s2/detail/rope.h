#pragma once

namespace s2 {
template <typename T>
struct convert;

template <typename encoding, typename... Ts, size_t... inds>
size_t get_string_length_(std::tuple<Ts...> ts, std::index_sequence<inds...>) {
  return (get_string_length1(std::get<inds>(ts)) + ... + 0);
}
template <typename encoding, typename It, typename... Ts, size_t... inds>
void get_string_contents_(It it, std::tuple<Ts...> ts, std::index_sequence<inds...>) {
  (get_string_content(it, std::get<inds>(ts)), ...);
}
template <typename encoding, typename... Ts>
size_t get_string_length(std::tuple<Ts...> t) {
  get_string_length_<encoding>(t, std::make_index_sequence<sizeof...(Ts)>());
}
template <typename encoding, typename It, typename... Ts>
void get_string_contents(It it, std::tuple<Ts...> t) {
  get_string_contents_<encoding>(it, t, std::make_index_sequence<sizeof...(Ts)>());
}

template <typename... Ts>
class rope {
public:
  rope(std::tuple<Ts...> t) : t(t) {}
  template <typename T2>
  rope<Ts..., T2> operator+(T2&& t2) && {
    return rope<Ts..., T2>(std::tuple_cat(std::move(t), std::tuple<T2>(std::forward<T2>(t2))));
  }
  std::tuple<Ts...> t;
};

}


