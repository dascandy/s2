#pragma once

#include <mutex>
#include <condition_variable>
#include <new>
#include <atomic>
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;
namespace s2 {

template <typename T, size_t N = 1024>
class async_queue {
public:
  void push(T&& value) noexcept {
    std::unique_lock<std::mutex> l(m);
    wcv.wait(l, [this]{ return ((write + 1) % N) != read; });
    size_t mine = write;
    write = (write + 1) % N;
    new (&storage()[mine])T(std::forward<decltype(value)>(value));
    rcv.notify_one();
  }
  T pop() noexcept {
    std::unique_lock<std::mutex> l(m);
    rcv.wait(l, [this]{ return read != write; });
    size_t mine = read;
    read = (read + 1) % N;
    T value = std::move(storage()[mine]);
    storage()[mine].~T();
    wcv.notify_one();
    return value;
  }
private:
  T* storage() { return reinterpret_cast<T*>(&storage_); }
  std::mutex m;
  std::condition_variable rcv;
  std::condition_variable wcv;
  size_t read, write;
  std::aligned_storage<sizeof(T) * N, alignof(T)> storage_;
};

class threadpool {
public:
  template <typename T>
  threadpool(T&& func, size_t threadcount = std::thread::hardware_concurrency()) 
  : threads(threadcount)
  {
    for (size_t n = 0; n < threadcount; n++)
      threads.emplace(threads.end(), func);
  }
  size_t count() const { return threads.size(); }
private:
  std::vector<std::thread> threads;
};

class default_executor {
public:
  default_executor() 
  : running(true)
  , threads_active(0)
  , pool_([this]{ run(); })
  {
  }
  ~default_executor() {
    int threads = pool_.count();
    running = false;
    // Schedule N blank tasks to allow all to exit
    for (size_t n = 0; n < threads; n++) {
      schedule([]{});
    }
    while (threads_active > 0) { std::this_thread::sleep_for(1ms); }
  }
  void run() {
    while (running) {
      auto task = q_.pop();
      task();
    }
  }
  void schedule(std::function<void()>&& task) {
    q_.push(std::move(task));
  }
  std::atomic<bool> running;
  std::atomic<int> threads_active;
  async_queue<std::function<void()>> q_;
  threadpool pool_;
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
private:
  void wait(std::unique_lock<std::mutex>& l) const {
    if (state == Empty) {
      cv.wait(l, [this]{ return state != Empty; });
    }
  }
  mutable std::mutex m;
  mutable std::condition_variable cv;
  enum { Empty, Value, Exception } state = Empty;
  union {
    std::aligned_storage<sizeof(T), alignof(T)> storage;
    std::exception_ptr eptr;
  };
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

template <typename... Ts>
const typename future<Ts...>::value_type& future<Ts...>::get() const {
  if constexpr (sizeof...(Ts) == 0) {
    return;
  } else if constexpr (sizeof...(Ts) == 1) {
    return std::get<0>(state->get());
  } else {
    return state->get();
  }
}

template <typename... Ts>
constexpr promise<Ts...>::promise() noexcept {
  state = new shared_state<std::tuple<Ts...>>();
  printf("%p\n", state);
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

template <typename Executor, typename F, typename... Args>
future<decltype(std::declval<F>()(std::declval<Args>()...))> async(Executor&& exec, F&& f, Args&&... args) {
  using rv = decltype(std::declval<F>()(std::declval<Args>()...));
  promise<rv> p;
  future<rv> rvalue = p.get_future();
  std::function<void()> task([=]() mutable {
    try {
      p.set_value(f(std::forward(args)...));
    } catch (...) {
      p.set_exception(std::current_exception());
    }
  });
  exec.schedule(std::move(task));
  return rvalue;
}

}


