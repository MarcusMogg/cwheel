
find_package(fmt CONFIG REQUIRED)

add_library (cwheel_lib STATIC
"hello.h" "hello.cc"    
"delegate.h"
 "thread_pool.h" "thread_pool.cc")

 target_compile_features(cwheel_lib PRIVATE cxx_std_17)

target_link_libraries(cwheel_lib
  PUBLIC
    fmt::fmt
)

target_include_directories(cwheel_lib
    PUBLIC
         ${PROJECT_SOURCE_DIR}/
)
