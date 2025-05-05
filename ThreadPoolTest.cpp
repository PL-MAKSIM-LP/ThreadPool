#include <gtest/gtest.h>

#include "LockFreeQueue.h"
#include "ThreadPool.h"

TEST(ThreadPoolTestTest, SimpleTask1) {
  for (int i = 0; i < 2; ++i) {
    std::cout << "Attempt #" << i + 1 << std::endl;

    std::atomic<int> x = 0;
    int result = 20000;
    ThreadPool pool;

    for (int i = 0; i < result; ++i) {
      pool.addTask<void>(1,
                         [&x]() { x.fetch_add(1, std::memory_order_relaxed); });
    }

    while (!pool.isQueueEmpty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_EQ(x.load(), result);
    std::cout << x << std::endl;
  }
}

TEST(ThreadPoolTestTest, SimpleTask2) {
  LockFreeQueue<int> lockFreeQueue;

  int result = 100;
  ThreadPool pool;
  for (int i = 1; i < result + 1; ++i) {
    pool.addTask<void>(1, [&lockFreeQueue, i]() {
      std::cout << "i = " << i << std::endl;
      lockFreeQueue.push(i);
    });
  }

  while (!pool.isQueueEmpty()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // lockFreeQueue.printAllData();
}

TEST(ThreadPoolTestTest, SimpleTask3) {
  LockFreeQueue<int> lockFreeQueue;

  int result = 100;
  for (int i = 1; i < result + 1; ++i) {
    lockFreeQueue.push(i);
  }

  lockFreeQueue.printAllData();
}
