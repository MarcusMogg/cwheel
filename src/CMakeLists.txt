project ("cwheel_lib")


# 引入fmtlib
include(FetchContent)
FetchContent_Declare(
  fmtlib
  URL https://github.com/fmtlib/fmt/archive/refs/tags/8.1.1.zip
)
FetchContent_MakeAvailable(fmtlib)


add_library (cwheel_lib STATIC
"hello.h" "hello.cc"    
"delegate.h"
 "thread_pool.h" "thread_pool.cc")

target_link_libraries(
  cwheel_lib
  fmt
)

target_include_directories(cwheel_lib
    PUBLIC
         ${PROJECT_SOURCE_DIR}/
)
