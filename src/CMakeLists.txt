
find_package(fmt CONFIG REQUIRED)

file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")
file(GLOB_RECURSE CC_FILES "*.cc" "*.cpp")

add_library (cwheel_lib STATIC ${HEADER_FILES} ${CC_FILES})
set_property(TARGET cwheel_lib PROPERTY CXX_STANDARD 20)

target_link_libraries(cwheel_lib
  PUBLIC
    fmt::fmt
)

target_include_directories(cwheel_lib
    PUBLIC
         ${PROJECT_SOURCE_DIR}/
)
