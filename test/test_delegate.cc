
#include <gtest/gtest.h>

#include "fmt/format.h"
#include "src/delegate.h"

using namespace cwheel;
using namespace std;
string normal_str;
string static_str;
string member_str;
string lambda_str;

static void Foo(int a) { normal_str = fmt::format("Call from normal function: {}", a); }

struct DelegateHandler {
  static void Bar(int a) { static_str = fmt::format("Call from static member function: {}", a); }

  void Bar2(int a) { member_str = fmt::format("Call from member function some_test={}: {}", some_test, a); }

  std::string some_test;
};

TEST(Delegate, FuncTest) {
  DefaultDelegateHost host;

  struct IntFunc {
    using Signature = void(int);
  };

  DelegateHandler test;
  test.some_test = "instance";

  host.Register<IntFunc>(Foo);
  host.Register<IntFunc>(&DelegateHandler::Bar);
  host.Register<IntFunc>([&test](int a) { test.Bar2(a); });  // don't like bind, lambda do same
  host.Register<IntFunc>([](int a) { lambda_str = fmt::format("Call from lambda expression: {}", a); });

  host.Invoke<IntFunc>(123);

  EXPECT_EQ(normal_str, "Call from normal function: 123");
  EXPECT_EQ(static_str, "Call from static member function: 123");
  EXPECT_EQ(member_str, "Call from member function some_test=instance: 123");
  EXPECT_EQ(lambda_str, "Call from lambda expression: 123");
}

TEST(Delegate, DeleteTest) {
  struct IntFunc {
    using Signature = void(int, int&);
  };

  const auto add = [](int in, int& out) { out += in; };
  int a = 1;

  DefaultDelegateHost host;
  host.Register<IntFunc>(add);
  host.Register<IntFunc>(add);

  host.Invoke<IntFunc>(a, a);

  EXPECT_EQ(a, 4);

  DefaultDelegateHost host2;
  host2.Register<IntFunc>(add);
  const auto it1 = host2.Register<IntFunc>(add);
  host2.Delete<IntFunc>(it1);
  a = 1;
  host2.Invoke<IntFunc>(a, a);

  EXPECT_EQ(a, 2);

  DefaultDelegateHost host3;
  host3.Register<IntFunc>(add);
  host3.Register<IntFunc>(add);
  host3.Clear<IntFunc>();
  a = 1;
  host3.Invoke<IntFunc>(a, a);

  EXPECT_EQ(a, 1);
}