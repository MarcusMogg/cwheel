
#include <gtest/gtest.h>

#include <format>

#include "src/function.hpp"

using namespace cwheel;
using namespace std;
namespace functestdetail {

string normal_str;
string static_str;
string member_str;
string lambda_str;

static void Foo(int a) { normal_str = std::format("Call from normal function: {}", a); }

struct DelegateHandler {
  static void Bar(int a) { static_str = std::format("Call from static member function: {}", a); }

  void Bar2(int a) { member_str = std::format("Call from member function some_test={}: {}", some_test, a); }

  std::string some_test;
};
}  // namespace functestdetail
using namespace functestdetail;

TEST(Function, FuncTest) {
  DelegateHandler test;
  test.some_test = "instance";

  FunctionPtr<void(int)> f;

  f = Foo;
  f(123);
  EXPECT_EQ(normal_str, "Call from normal function: 123");

  f = &DelegateHandler::Bar;
  f(123);
  EXPECT_EQ(static_str, "Call from static member function: 123");

  f = [&test](int a) { test.Bar2(a); };
  f(123);
  EXPECT_EQ(member_str, "Call from member function some_test=instance: 123");

  f = [](int a) { lambda_str = std::format("Call from lambda expression: {}", a); };
  f(123);
  EXPECT_EQ(lambda_str, "Call from lambda expression: 123");
}

TEST(Function, NullTest) {
  FunctionPtr<void(int, int&)> f;

  const auto add = [](int in, int& out) { out += in; };
  int a = 1;

  f(a, a);
  EXPECT_EQ(a, 1);

  f = add;
  f(a, a);
  EXPECT_EQ(a, 2);
}