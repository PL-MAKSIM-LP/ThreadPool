#pragma once

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

#include "TaskQueue.h"

class ThreadPool {
 public:
  ThreadPool() : isWork(true) {
    const size_t numThreads = std::thread::hardware_concurrency();
    // const size_t numThreads = 1;
    threads.reserve(numThreads);

    for (size_t i = 0; i < numThreads; ++i) {
      threads.emplace_back([this]() { workerThread(); });
    }

    std::cout << "ThreadPool started with " << numThreads << " threads."
              << std::endl;
  }

  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(mutex);
      isWork = false;
    }
    for (size_t i = 0; i < threads.size(); ++i) {
      cv.notify_one();
    }

    for (auto& thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  template <typename ResultType, typename F, typename... Args>
  void addTask(size_t priority, F&& func, Args&&... args) {
    auto task = TypeSafeTask<ResultType>(
        [f = std::forward<F>(func),
         ... args = std::forward<Args>(args)]() mutable { return f(args...); });

    {
      std::lock_guard<std::mutex> lock(mutex);
      if (!isWork) {
        throw std::runtime_error("ThreadPool is stopped!");
      }
      taskQueue.addTask(priority, std::move(task));
    }

    cv.notify_one();
  }

  bool isQueueEmpty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return taskQueue.empty();
  }

 private:
  void workerThread() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    while (true) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this]() { return !taskQueue.empty() || !isWork; });

        if (!isWork && taskQueue.empty()) {
          return;
        }

        if (!taskQueue.getTask(task)) {
          continue;
        }
      }

      try {
        if (task) {
          task();
        }
      } catch (const std::exception& e) {
      }
    }
  }

  std::atomic<bool> isWork;
  TypeSafeTaskQueue taskQueue;
  std::vector<std::thread> threads;
  mutable std::mutex mutex;
  std::condition_variable cv;
};
