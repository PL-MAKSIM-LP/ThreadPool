#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <queue>

#include "TypeSafeTask.h"

class TypeSafeTaskQueue {
 private:
  struct PriorityTask {
    size_t priority;
    std::shared_ptr<void> task;
    std::function<void()> executor;

    template <typename ResultType, typename... Args>
    PriorityTask(size_t p, TypeSafeTask<ResultType, Args...>&& task)
        : priority(p),
          task(std::make_shared<TypeSafeTask<ResultType, Args...>>(
              std::move(task))) {
      auto typed_task =
          static_cast<TypeSafeTask<ResultType, Args...>*>(this->task.get());

      executor = [typed_task]() {
        if constexpr (std::is_void_v<ResultType>) {
          typed_task->execute();

        } else {
          auto result = typed_task->execute();

        }
      };
    }

    bool operator<(const PriorityTask& other) const {
      return priority > other.priority;
    }
  };

 public:
  template <typename ResultType, typename... Args>
  void addTask(size_t priority, TypeSafeTask<ResultType, Args...>&& task) {
    if (priority < 1 || priority > 10) {
      throw std::invalid_argument("Priority must be between 1 and 10!");
    }

    std::lock_guard<std::mutex> lock(mutex);
    taskQueue.emplace(priority, std::move(task));
  }

  bool getTask(std::function<void()>& task) {
    std::lock_guard<std::mutex> lock(mutex);
    if (taskQueue.empty()) {
      return false;
    }

    task = std::move(taskQueue.top().executor);
    taskQueue.pop();
    return true;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return taskQueue.empty();
  }

 private:
  std::priority_queue<PriorityTask> taskQueue;
  mutable std::mutex mutex;
};
