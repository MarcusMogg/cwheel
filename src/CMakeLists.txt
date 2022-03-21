project ("cwheel_lib")

add_library (cwheel_lib STATIC
"hello.h" "hello.cc"    
"delegate.h"
)

target_include_directories(cwheel_lib
    PUBLIC
         ${PROJECT_SOURCE_DIR}/
)
