#pragma once

#include <concepts>
#include <utility>

namespace cwheel {
template <std::default_initializable T, class... Args>
inline T FindFirstNotDefault(T&& v, Args&&... args) {
  if constexpr (sizeof...(args) == 0) {
    return v;
  } else {
    if (v == T{}) {
      return FindFirstNotDefault<T>(std::forward<Args>(args)...);
    }
    return v;
  }
}

// check a0 >= a1 >= a2 ....
template <std::totally_ordered Arg0, std::totally_ordered... Args>
bool IsDecrease(const Arg0& a0, const Args&... args) {
  if constexpr (sizeof...(args) == 0) {
    return true;
  } else {
    using CommonType = std::common_type_t<Arg0, Args...>;

    CommonType tmp0 = a0, tmp1 = a0;
    return ((tmp0 = tmp1, tmp1 = args, tmp0 >= tmp1) && ...);
  }
}

}  // namespace cwheel