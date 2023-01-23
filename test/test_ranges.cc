#include "gtest/gtest.h"
#include "src/ranges.hpp"

int F1(int a) { return a + 1; }
int F2(int a) { return a * 2; }

TEST(Ranges, ComposeTest) {
  auto f1 = cwheel::Compose(F1, F2);
  EXPECT_EQ(f1(1), 3);

  auto f2 = cwheel::Compose(F2, F1);
  EXPECT_EQ(f2(1), 4);
}