#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <queue>

#include <uuid/uuid.h>
#include <string>

#include "TypeSafeTask.h"

static int _id = 0;

class TypeSafeTaskQueue {
 private:
  struct PriorityTask {
    size_t priority;
    std::shared_ptr<void> task;
    std::function<void()> executor;
    int id;

    template <typename ResultType, typename... Args>
    PriorityTask(size_t p, TypeSafeTask<ResultType>&& task)
        : priority(p),
          task(std::make_shared<TypeSafeTask<ResultType>>(std::move(task))) {
      auto typed_task =
          static_cast<TypeSafeTask<ResultType>*>(this->task.get());

      // id = generateUUID();
      id = ++_id;

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

    std::string generateUUID() {
      uuid_t uuid;
      uuid_generate(uuid);
      char str[37];
      uuid_unparse(uuid, str);
      return std::string(str);
    }
  };

 public:
  template <typename ResultType, typename... Args>
  void addTask(size_t priority, TypeSafeTask<ResultType>&& task) {
    if (priority < 1 || priority > 10) {
      throw std::invalid_argument("Priority must be between 1 and 10!");
    }

    std::lock_guard<std::mutex> lock(mutex);
    taskQueue.emplace(priority, std::move(task));
  }

  bool getTask(std::function<void()>& task, std::string& str) {
    std::lock_guard<std::mutex> lock(mutex);
    if (taskQueue.empty()) {
      return false;
    }

    task = std::move(taskQueue.top().executor);

    str = std::to_string(taskQueue.top().id);

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
