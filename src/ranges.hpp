#pragma once

#include <concepts>
#include <functional>
#include <ranges>
#include <type_traits>
#include <utility>

namespace cwheel {
struct ComposeOp {
  template <class _Fn1, class _Fn2, class... _Args>
  constexpr auto operator()(_Fn1&& __f1, _Fn2&& __f2, _Args&&... __args) const {
    return std::invoke(std::forward<_Fn1>(__f1), std::invoke(std::forward<_Fn2>(__f2), std::forward<_Args>(__args)...));
  }
};
template <class _Fn1, class _Fn2>
struct ComposeT : std::__perfect_forward<ComposeOp, _Fn1, _Fn2> {
  using std::__perfect_forward<ComposeOp, _Fn1, _Fn2>::__perfect_forward;
};
template <class _Fn1, class _Fn2>
constexpr auto Compose(_Fn1&& __f1, _Fn2&& __f2) {
  return ComposeT<std::decay_t<_Fn1>, std::decay_t<_Fn2>>(std::forward<_Fn1>(__f1), std::forward<_Fn2>(__f2));
}

template <typename T>
struct RangeAdaptorClosure;

template <class _Fn>
struct RangeAdaptorClosureT : _Fn, RangeAdaptorClosure<RangeAdaptorClosureT<_Fn>> {
  constexpr explicit RangeAdaptorClosureT(_Fn&& __f) : _Fn(std::move(__f)) {}
};

template <typename T>
struct RangeAdaptorClosure {
  template <std::ranges::viewable_range View, typename Closure>
    requires std::invocable<Closure, View> && std::same_as<T, std::remove_cv_t<Closure>>
  friend constexpr decltype(auto) operator|(View&& view, Closure&& closure) {
    return std::invoke(std::forward<Closure>(closure), std::forward<View>(view));
  }

  template <typename Closure, typename AnotherClosure>
  friend constexpr decltype(auto) operator|(Closure&& closure, AnotherClosure&& another_closure) {
    return Compose(std::forward<AnotherClosure>(another_closure), std::forward<Closure>(closure));
  }
};

}  // namespace cwheel