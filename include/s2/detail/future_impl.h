#pragma once

#include "default_executor.h"
#include "future.h"
#include "promise.h"

namespace s2 {

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


