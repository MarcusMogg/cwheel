#pragma once

#include <concepts>
#include <functional>
#include <ranges>
#include <type_traits>
#include <utility>

namespace cwheel {
template <typename Fn1, typename Fn2>
struct ComposeOp {
  Fn1 f1;
  Fn2 f2;

  explicit constexpr ComposeOp(Fn1&& __f1, Fn2&& __f2)
      : f1(std::forward<Fn1>(__f1)), f2(std::forward<Fn2>(__f2)) {}

  template <typename... _Args>
  constexpr auto operator()(_Args&&... __args) const {
    return std::invoke(f1, std::invoke(f2, std::forward<_Args>(__args)...));
  }
};

template <typename Fn1, typename Fn2>
constexpr auto Compose(Fn1&& __f1, Fn2&& __f2) {
  return ComposeOp<std::decay_t<Fn1>, std::decay_t<Fn2>>(
      std::forward<Fn1>(__f1), std::forward<Fn2>(__f2));
}

template <typename T>
struct RangeAdaptorClosure;

template <typename Fn>
struct RangeAdaptorClosureT : Fn, RangeAdaptorClosure<RangeAdaptorClosureT<Fn>> {
  constexpr explicit RangeAdaptorClosureT(Fn&& __f) : Fn(std::move(__f)) {}
};

template <typename T>
concept RangeAdaptorClosureAble =
    std::derived_from<std::remove_cvref_t<T>, RangeAdaptorClosure<std::remove_cvref_t<T>>>;

template <typename T>
struct RangeAdaptorClosure {
  template <std::ranges::viewable_range View, RangeAdaptorClosureAble Closure>
  requires std::invocable<Closure, View> && std::same_as<T, std::remove_cvref_t<Closure>>
  friend constexpr decltype(auto) operator|(View&& view, Closure&& closure) {
    return std::invoke(std::forward<Closure>(closure), std::forward<View>(view));
  }

  template <RangeAdaptorClosureAble Closure, RangeAdaptorClosureAble AnotherClosure>
  requires std::same_as<T, std::remove_cvref_t<Closure>>
  friend constexpr auto operator|(Closure&& closure, AnotherClosure&& another_closure) {
    return Compose(std::forward<AnotherClosure>(another_closure), std::forward<Closure>(closure));
  }
};

template <typename T>
concept CanRef = requires(T&& t) {
  std::ranges::ref_view{std::forward<T>(t)};
};
template <typename T>
concept CanOwn = requires(T&& t) {
  std::ranges::owning_view{std::forward<T>(t)};
};

template <typename T>
struct AlwaysFalse : public std::false_type {};

struct AllView : public RangeAdaptorClosure<AllView> {
  template <std::ranges::viewable_range T>
  constexpr auto operator()(T&& t) const {
    if constexpr (std::ranges::view<std::decay_t<T>>) {
      return std::forward<T>(t);
    } else if constexpr (CanRef<T>) {
      return std::ranges::ref_view{std::forward<T>(t)};
    } else if constexpr (CanOwn<T>) {
      return std::ranges::owning_view{std::forward<T>(t)};
    } else {
      static_assert(AlwaysFalse<T>(), "Should be unreachable");
    }
  }
};

inline constexpr auto all = AllView{};

}  // namespace cwheel