#include "hello.h"

#include "fmt/format.h"
std::string hello() { return fmt::format("{}", "hello"); }