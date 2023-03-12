// #include <array>
// #include <iostream>
// #include <vector>

// #include "gtest/gtest.h"
// #include "src/ranges.hpp"

// int F1(int a) { return a + 1; }
// int F2(int a) { return a * 2; }

// TEST(Ranges, ComposeTestBasicFunc) {
//   auto f1 = cwheel::Compose(F1, F2);
//   EXPECT_EQ(f1(1), 3);

//   auto f2 = cwheel::Compose(F2, F1);
//   EXPECT_EQ(f2(1), 4);
// }

// TEST(Ranges, ComposeTestLambda) {
//   auto f1 = cwheel::Compose(F1, [](int a) { return a * 2; });
//   EXPECT_EQ(f1(1), 3);

//   auto f2 = cwheel::Compose(F2, [](int a) { return a + 1; });
//   EXPECT_EQ(f2(1), 4);
// }

// TEST(Ranges, AllTest) {
//   auto a = std::vector{1, 2, 3, 4};
//   auto aa = std::span{a};

//   for (int cur = 1; const auto& i : cwheel::all(aa)) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }
//   for (int cur = 1; const auto& i : aa | cwheel::all) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }

//   for (int cur = 1; const auto& i : aa | cwheel::all | cwheel::all) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }

//   for (int cur = 1; const auto& i : cwheel::all(a)) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }
//   for (int cur = 1; const auto& i : a | cwheel::all) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }
//   for (int cur = 1; const auto& i : a | cwheel::all | cwheel::all) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }

//   for (int cur = 1; const auto& i : std::vector{1, 2, 3, 4} | cwheel::all) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }

//   auto g = []() { return std::array{1, 2, 3, 4}; };
//   for (int cur = 1; const auto& i : g() | cwheel::all) {
//     EXPECT_EQ(cur, i);
//     cur++;
//   }
// }