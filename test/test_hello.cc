#include <gtest/gtest.h>

#include "src/hello.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  EXPECT_EQ(hello(), std::string("hello"));

  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}