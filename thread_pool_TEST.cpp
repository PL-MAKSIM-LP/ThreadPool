#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <algorithm>
#include "ThreadPool.h"

// Тест TaskQueue
class TaskQueueTest : public ::testing::Test {
protected:
    TaskQueue queue;
};

TEST_F(TaskQueueTest, EmptyInitially) {
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}
