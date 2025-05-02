#pragma once

#include <functional>
#include <utility>

template <typename ResultType, typename... Args>
class TypeSafeTask {
 public:
  template <typename Callable>
  explicit TypeSafeTask(Callable&& func) : task(std::forward<Callable>(func)) {
    static_assert(std::is_invocable_r<ResultType, Callable, Args...>::value,
                  "Callable does not match the expected signature!");
  }

  ResultType execute(Args... args) {
    if (!task) {
      return;
    }
    return task(std::forward<Args>(args)...);
  }

  bool isValid() const { return static_cast<bool>(task); }

 private:
  std::function<ResultType(Args...)> task;
};
