
#include <gtest/gtest.h>

#include <format>

#include "src/sequence.hpp"

using namespace cwheel;
using namespace std;

TEST(Sequence, FindFirstNotDefault) {
  EXPECT_EQ(FindFirstNotDefault(0, 1, 5, 4), 1);
  EXPECT_EQ(FindFirstNotDefault(0), 0);
  EXPECT_EQ(FindFirstNotDefault(0, 0), 0);
  EXPECT_EQ(FindFirstNotDefault(0, 1, 0), 1);
  EXPECT_EQ(FindFirstNotDefault(1, 2, 0), 1);

  EXPECT_EQ(FindFirstNotDefault<std::string>("a", "b", "c"), std::string("a"));
  EXPECT_EQ(FindFirstNotDefault<std::string>("", "b", "c"), std::string("b"));
  EXPECT_EQ(FindFirstNotDefault<std::string>("", "b", ""), std::string("b"));
  EXPECT_EQ(FindFirstNotDefault<std::string>("", "", ""), std::string(""));

  EXPECT_EQ(FindFirstNotDefault<std::string_view>("a", "b", "c"), std::string_view("a"));
  EXPECT_EQ(FindFirstNotDefault<std::string_view>("", "b", "c"), std::string_view("b"));
  EXPECT_EQ(FindFirstNotDefault<std::string_view>("", "b", ""), std::string_view("b"));
  EXPECT_EQ(FindFirstNotDefault<std::string_view>("", "", ""), std::string_view(""));
}

TEST(Sequence, IsDecreaseTest) {
  EXPECT_FALSE(IsDecrease(0, 1, 5, 4));
  EXPECT_EQ(IsDecrease(0, 1, 0), false);

  EXPECT_EQ(IsDecrease(0), true);
  EXPECT_EQ(IsDecrease(0, 0), true);
  EXPECT_EQ(IsDecrease(2, 1, 0), true);
  EXPECT_EQ(IsDecrease(2, 2, 1, 1, 0), true);

  EXPECT_EQ(IsDecrease<std::string_view>("a", "b", "c"), false);
  EXPECT_EQ(IsDecrease<std::string_view>("a", "c", "b"), false);
  EXPECT_EQ(IsDecrease<std::string_view>("b", "c", "a"), false);
  EXPECT_EQ(IsDecrease<std::string_view>("b", "a", "c"), false);
  EXPECT_EQ(IsDecrease<std::string_view>("c", "a", "b"), false);
  EXPECT_EQ(IsDecrease<std::string_view>("c", "b", "a"), true);
  EXPECT_EQ(IsDecrease<std::string_view>("b", "b", "a"), true);
  EXPECT_EQ(IsDecrease<std::string_view>("a", "a", "a"), true);
  EXPECT_EQ(IsDecrease<std::string_view>("b"), true);
}
