cmake_minimum_required (VERSION 3.14)

# 引入gtest https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB test_files test_*.cc)

add_executable(
  cwheel_test
  ${test_files}
)
target_link_libraries(
  cwheel_test
  cwheel_lib
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(cwheel_test)