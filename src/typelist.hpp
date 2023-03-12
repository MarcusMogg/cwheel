#pragma once

#include <vcruntime.h>
namespace cwheel {

template <class... T>
class TypeList {};

template <class T>
class FrontT;
template <class Head, class... Tail>
class FrontT<TypeList<Head, Tail...>> {
  using Type = Head;
};
template <class List>
using Front = typename FrontT<List>::Type;

template <class List, class NewElement>
class PushFrontT;
template <class... Elements, class NewElement>
class PushFrontT<TypeList<Elements...>, NewElement> {
  using Type = TypeList<NewElement, Elements...>;
};
template <class List, class NewElement>
using PushFront = typename PushFrontT<List, NewElement>::Type;

template <class List, class NewElement>
class PushBackT;
template <class... Elements, class NewElement>
class PushBackT<TypeList<Elements...>, NewElement> {
  using Type = TypeList<Elements..., NewElement>;
};
template <class List, class NewElement>
using PushBack = typename PushBackT<List, NewElement>::Type;

template <class List>
class PopFrontT;
template <class Front, class... Elements>
class PopFrontT<TypeList<Front, Elements...>> {
  using Type = TypeList<Elements...>;
};
template <class List>
using PopFront = typename PopFrontT<List>::Type;

template <class List, size_t N>
class NthElementT;
template <class List>
class NthElementT<List, 0> : FrontT<List> {};
template <class List, size_t N>
class NthElementT : public NthElementT<PopFront<List>, N - 1> {};
template <class List, size_t N>
using NthElement = typename NthElementT<List, N>::Type;

}  // namespace cwheel