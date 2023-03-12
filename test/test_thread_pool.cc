#include <gtest/gtest.h>

#include <chrono>

#include "src/thread_pool.h"

using cwheel::ThreadPool;

TEST(ThreadPoolTest, SingleThread) {
  int a = 0;
  const auto add = [&a]() { a += 1; };
  do {
    ThreadPool pool(1);
    pool.AddTask([&a]() { a += 1; });
    pool.AddTask([&a]() { a += 1; });
    pool.AddTask([&a]() { a += 1; });
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(200ms);
  } while (false);

  EXPECT_EQ(a, 3);
}