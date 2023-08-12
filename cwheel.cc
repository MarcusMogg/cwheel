#include <iostream>

#include "src/delegate.hpp"
#include "src/function.hpp"
#include "src/iterator.hpp"
#include "src/sequence.hpp"
#include "src/thread_pool.h"
#include "src/typelist.hpp"

int main() {
#if defined(__clang__)
  std::cout << "Hello clang." << std::endl;
#elif defined(__GNUC__) || defined(__GNUG__)
  std::cout << "Hello gcc." << std::endl;
#elif defined(_MSC_VER)
  std::cout << "Hello msvc." << std::endl;
#endif
  return 0;
}
