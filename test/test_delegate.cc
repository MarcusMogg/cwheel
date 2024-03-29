
#include <gtest/gtest.h>

#include <format>

#include "src/delegate.hpp"

namespace delegatetestdetail {

std::string normal_str;
std::string static_str;
std::string member_str;
std::string lambda_str;

void Foo(int a) { normal_str = std::format("Call from normal function: {}", a); }

struct DelegateHandler {
  static void Bar(int a) { static_str = std::format("Call from static member function: {}", a); }

  void Bar2(int a) {
    member_str = std::format("Call from member function some_test={}: {}", some_test, a);
  }

  std::string some_test;
};
}  // namespace delegatetestdetail

TEST(Delegate, FuncTest) {
  cwheel::DefaultDelegateHost host;

  struct IntFunc {
    using Signature = void(int);
  };

  delegatetestdetail::DelegateHandler test;
  test.some_test = "instance";

  host.Register<IntFunc>(delegatetestdetail::Foo);
  host.Register<IntFunc>(&delegatetestdetail::DelegateHandler::Bar);
  host.Register<IntFunc>([&test](int a) { test.Bar2(a); });  // don't like bind, lambda do same
  host.Register<IntFunc>([](int a) {
    delegatetestdetail::lambda_str = std::format("Call from lambda expression: {}", a);
  });

  host.Invoke<IntFunc>(123);

  EXPECT_EQ(delegatetestdetail::normal_str, "Call from normal function: 123");
  EXPECT_EQ(delegatetestdetail::static_str, "Call from static member function: 123");
  EXPECT_EQ(delegatetestdetail::member_str, "Call from member function some_test=instance: 123");
  EXPECT_EQ(delegatetestdetail::lambda_str, "Call from lambda expression: 123");
}

TEST(Delegate, DeleteTest) {
  struct IntFunc {
    using Signature = void(int, int&);
  };

  const auto add = [](int in, int& out) { out += in; };
  int a = 1;

  cwheel::DefaultDelegateHost host;
  host.Register<IntFunc>(add);
  host.Register<IntFunc>(add);

  host.Invoke<IntFunc>(a, a);

  EXPECT_EQ(host.GetDelegatePool().size(), 1);
  EXPECT_EQ(a, 4);

  cwheel::DefaultDelegateHost host2;
  host2.Register<IntFunc>(add);
  const auto it1 = host2.Register<IntFunc>(add);
  host2.Delete<IntFunc>(it1);
  a = 1;
  host2.Invoke<IntFunc>(a, a);

  EXPECT_EQ(a, 2);

  cwheel::DefaultDelegateHost host3;
  host3.Register<IntFunc>(add);
  host3.Register<IntFunc>(add);
  host3.Clear<IntFunc>();
  a = 1;
  host3.Invoke<IntFunc>(a, a);

  EXPECT_EQ(a, 1);
}