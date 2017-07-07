#pragma once

#include <mutex>
#include <condition_variable>
#include <new>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::literals::chrono_literals;
namespace s2 {

template <typename T, size_t N = 1024>
class async_queue {
public:
  async_queue() {}
  async_queue(const async_queue&) = delete;
  async_queue(async_queue&&) = delete;
  async_queue& operator=(const async_queue&) = delete;
  async_queue& operator=(async_queue&&) = delete;
  ~async_queue() {
    while (read != write) {
      try {
        pop();
      } catch (...) {}
    }
  }
  void push(T&& value) noexcept {
    {
      std::unique_lock<std::mutex> l(m);
      wcv.wait(l, [this]{ return !full(); });
      size_t mine = write;
      write = (write + 1) % N;
      new (&storage()[mine])T(std::forward<decltype(value)>(value));
    }
    rcv.notify_one();
  }
  T pop() {
    std::unique_lock<std::mutex> l(m);
    rcv.wait(l, [this]{ return stop || !empty(); });
    if (empty()) throw std::exception();
    size_t mine = read;
    T value = std::move(storage()[mine]);
    storage()[mine].~T();
    read = (read + 1) % N;
    wcv.notify_one();
    return value;
  }
  void notify() {
    {
      std::unique_lock<std::mutex> l(m);
      stop = true;
    }
    rcv.notify_all();
  }
private:
  bool empty() const noexcept {
    return read == write;
  }
  bool full() const noexcept {
    return ((write+1)%N) == read;
  }
  T* storage() { return reinterpret_cast<T*>(&storage_); }
  std::mutex m;
  std::condition_variable rcv;
  std::condition_variable wcv;
  size_t read = 0, write = 0;
  bool stop = false;
  std::aligned_storage_t<sizeof(T) * N, alignof(T)> storage_;
};

class default_executor {
public:
  default_executor() 
  {
    for (size_t n = 0; n < std::thread::hardware_concurrency(); n++) {
      threads.emplace_back([this]{ run(); });
    }
  }
  default_executor(const default_executor&) = delete;
  default_executor(default_executor&&) = delete;
  default_executor& operator=(const default_executor&) = delete;
  default_executor& operator=(default_executor&&) = delete;
  ~default_executor() {
    q_.notify();
    for (auto& t : threads) {
      t.join();
    }
  }
  void run() {
    try {
      while (1) {
        auto task = q_.pop();
        task();
      }
    } catch(const std::exception& e) {
    }
  }
  void schedule(std::function<void()>&& task) {
    q_.push(std::move(task));
  }
  async_queue<std::function<void()>> q_;
  std::vector<std::thread> threads;
};

default_executor& get_default_executor() {
  static default_executor ex;
  return ex;
}

template <typename T>
class shared_state {
public:
  shared_state() 
  : state(Empty)
  {
  }
  shared_state(const shared_state&) = delete;
  shared_state(shared_state&&) = delete;
  shared_state& operator=(const shared_state&) = delete;
  shared_state& operator=(shared_state&&) = delete;
  ~shared_state() {
    switch(state) {
      case Value:
        get()->~T();
        break;
      case Exception:
        eptr.~exception_ptr();
        break;
    }
  }

  bool isset() const noexcept {
    std::unique_lock<std::mutex> l(m);
    return state != Empty;
  }
  bool iserror() const noexcept {
    std::unique_lock<std::mutex> l(m);
    return state == Exception;
  }

  const T& get() const {
    std::unique_lock<std::mutex> l(m);
    wait(l);
    if (state != Value)
      std::rethrow_exception(eptr);
    return reinterpret_cast<const T&>(storage);
  }

  const std::exception_ptr& get_error() const {
    std::unique_lock<std::mutex> l(m);
    wait(l);
    if (state != Exception)
      // shout.
    return eptr;
  }

  void set_value(T&& value) {
    std::unique_lock<std::mutex> l(m);
    if (state != Empty) {
      // TODO: report value already set
    }
    new(&reinterpret_cast<T&>(storage))T(std::forward<decltype(value)>(value));
    state = Value;
    cv.notify_all();
  }
  void set_error(std::exception_ptr&& ex) {
    std::unique_lock<std::mutex> l(m);
    if (state != Empty) {
      // TODO: report value already set
    }
    new(&eptr)std::exception_ptr(std::make_exception_ptr(std::forward<decltype(ex)>(ex)));
    state = Exception;
    cv.notify_all();
  }
  void wait(std::unique_lock<std::mutex>& l) const {
    if (state == Empty) {
      cv.wait(l, [this]{ return state != Empty; });
    }
  }
private:
  mutable std::mutex m;
  mutable std::condition_variable cv;
  enum { Empty, Value, Exception } state = Empty;
  union {
    std::aligned_storage<sizeof(T), alignof(T)> storage;
    std::exception_ptr eptr;
  };
};

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

template <typename Executor, typename F, typename... Args>
future<decltype(std::declval<F>()(std::declval<Args>()...))> async(Executor&& exec, F&& f, Args&&... args) {
  using rv = decltype(std::declval<F>()(std::declval<Args>()...));
  promise<rv> p;
  future<rv> rvalue = p.get_future();
  std::function<void()> task([=, p = std::move(p)]() mutable {
    p.run_async(f, std::forward<decltype(args)>(args)...);
  });
  exec.schedule(std::move(task));
  return rvalue;
}

}


