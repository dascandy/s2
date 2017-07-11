#pragma once

#include <thread>
#include <s2/functional>
#include <s2/vector>
#include "async_queue.h"

namespace s2 {

class default_executor {
public:
  default_executor() 
  {
    threads.reserve(std::thread::hardware_concurrency());
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
  void schedule(function<void()>&& task) {
    q_.push(std::move(task));
  }
  async_queue<function<void()>> q_;
  vector<std::thread> threads;
};

default_executor& get_default_executor() {
  static default_executor ex;
  return ex;
}

}


