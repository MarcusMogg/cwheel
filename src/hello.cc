#include "hello.h"

#include <fmt/core.h>
std::string hello() { return fmt::format("{}", "hello"); }