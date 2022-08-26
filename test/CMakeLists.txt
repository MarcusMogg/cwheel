find_package(GTest CONFIG REQUIRED)

enable_testing()

file(GLOB test_files test_*.cc)

add_executable(
  cwheel_test
  ${test_files}
)

target_compile_features(cwheel_test PRIVATE cxx_std_17)

target_link_libraries(cwheel_test
PRIVATE
  cwheel_lib
  GTest::gmock 
  GTest::gtest 
  GTest::gmock_main 
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cwheel_test)
