#include <gtest/gtest.h>

#include "ThreadPool.h"

TEST(ThreadPoolTestTest, SimpleTask1) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "Attempt #" << i + 1 << std::endl;

    std::atomic<int> x = 0;

    auto foo = [&x]() { ++x; };

    int result = 20000;
    ThreadPool pool;

    for (int i = 0; i < result; ++i) {
      pool.addTask<void>(1, foo);
    }

    while (!pool.isQueueEmpty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    EXPECT_EQ(x, result);
  }
}
