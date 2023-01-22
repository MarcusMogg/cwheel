#pragma once

#include <concepts>
#include <iterator>

namespace cwheel {

template <class _Tp>
using __with_reference = _Tp&;

template <class _Tp>
concept Referenceable = requires {
  typename __with_reference<_Tp>;
};

template <typename T>
concept input_or_output_iterator = requires {
  // 可以单向递增
  requires requires(T it) {
    it++;
    { ++it } -> std::same_as<T&>;
  };
  // 可以解引用
  requires requires(T it) {
    { *it } -> Referenceable;
  };
  // 可以比较相等
  std::equality_comparable<T>;
};

template <typename T>
concept input_iterator = requires {
  input_or_output_iterator<T>;
  std::indirectly_readable<T>;
};

template <typename T>
concept forward_iterator = requires {
  input_iterator<T>;
  std::incrementable<T>;  // TODO：这里没理解为什么 incrementable 就可以实现多趟
};

template <typename T>
concept bidirectional_iterator = requires {
  forward_iterator<T>;
  requires requires(T i) {
    { --i } -> std::same_as<T&>;
    { i-- } -> std::same_as<T>;
  };
};

template <typename T>
concept random_access_iterator = requires {
  bidirectional_iterator<T>;
  requires requires(T i, const T j, const std::iter_difference_t<T> n) {
    { i += n } -> std::same_as<T&>;
    { j + n } -> std::same_as<T>;
    { n + j } -> std::same_as<T>;
    { i -= n } -> std::same_as<T&>;
    { j - n } -> std::same_as<T>;
    { j[n] } -> std::same_as<std::iter_reference_t<T>>;
  };
};

}  // namespace cwheel