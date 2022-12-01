#include <gtest/gtest.h>

#include "src/hello.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  EXPECT_EQ(hello(), std::string("hello"));

  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

int main(int argc, char** argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}