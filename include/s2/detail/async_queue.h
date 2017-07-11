#pragma once

#include <mutex>
#include <condition_variable>
#include <new>

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

}


