#pragma once

#include "shared_state.h"

namespace s2 {

template <>
class future<void> {
public:
  using value_type = void;
  constexpr future() noexcept : state(nullptr) {}
  future(future&&) noexcept = default;
  future& operator=(future&&) noexcept = default;
  future(const future&) noexcept = default;
  future& operator=(const future&) noexcept = default;

  template <typename Ex, typename C>
  auto then(Ex&& ex, C&& c);
  template <typename C>
  auto then(C&& c) { return (*this).then(get_default_executor(), ::std::forward<decltype(c)>(c)); }
  bool ready() const noexcept;
  bool valid() const noexcept { return state != nullptr; }
  void get() const;
private:
  future(shared_state<std::tuple<>>* state) : state(state) {}
  friend class promise<void>;
  shared_state<std::tuple<>> *state;
};

template <typename... Ts>
template <typename... Us>
future<Ts...>::future(Us&&... us) {
  // TODO: when_all impl
}

template <typename... Ts>
template <typename Ex, typename C>
auto future<Ts...>::then(Ex&& ex, C&& c) {

}

template <typename... Ts>
bool future<Ts...>::ready() const noexcept { 
  return state->isset(); 
}

void future<void>::get() const {
  state->get();
}

template <typename... Ts>
auto future<Ts...>::get() const {
  if constexpr (sizeof...(Ts) == 1) {
    return std::get<0>(state->get());
  } else {
    return state->get();
  }
}

}


