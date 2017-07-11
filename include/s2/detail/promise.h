#pragma once

#include "shared_state.h"

namespace s2 {

template <>
class promise<void> {
public:
  promise() noexcept;
  future<void> get_future();
  void set_value();
  void set_exception(std::exception_ptr&& error);
  template <typename F, typename... Args>
  void run_async(F&& f, Args&&... args);
private:
  shared_state<std::tuple<>> *state;

};

template <typename... Ts>
promise<Ts...>::promise() noexcept {
  state = new shared_state<std::tuple<Ts...>>();
}

template <typename... Ts>
future<Ts...> promise<Ts...>::get_future() {
  return future<Ts...>(state);
}

template <typename... Ts>
template <typename... Us>
void promise<Ts...>::set_value(Us&&... us) {
  state->set_value(std::tuple<Ts...>(std::forward<decltype(us)>(us)...));
}

template <typename... Ts>
void promise<Ts...>::set_exception(std::exception_ptr &&error) {
  state->set_error(std::forward<decltype(error)>(error));
}

promise<void>::promise() noexcept 
: state(new shared_state<std::tuple<>>())
{
}

future<void> promise<void>::get_future() {
  return future<void>(state);
}

void promise<void>::set_value() {
  state->set_value(std::tuple<>());
}

void promise<void>::set_exception(std::exception_ptr &&error) {
  state->set_error(std::forward<decltype(error)>(error));
}

template <typename... Ts>
template <typename F, typename... Args>
void promise<Ts...>::run_async(F&& f, Args&&... args) {
  try {
    set_value(f(std::forward<decltype(args)>(args)...));
  } catch (...) {
    set_exception(std::current_exception());
  }
}

template <typename F, typename... Args>
void promise<void>::run_async(F&& f, Args&&... args) {
  try {
    f(std::forward(args)...);
    set_value();
  } catch (...) {
    set_exception(std::current_exception());
  }
}

}


