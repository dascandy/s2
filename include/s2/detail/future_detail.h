#pragma once

namespace s2 {

namespace detail {
  template <typename... Ts>
  struct future_type_helper { typedef ::std::tuple<Ts...> type; };
  template <typename T>
  struct future_type_helper<T> { typedef T type; };
  template <> 
  struct future_type_helper<> { typedef void type; };
  template <typename... Ts>
  using future_type = typename future_type_helper<Ts...>::type;
}

}


