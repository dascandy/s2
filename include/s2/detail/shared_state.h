#pragma once

#include <mutex>
#include <condition_variable>
#include <new>

namespace s2 {

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
      throw std::exception();
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

}


