# CMakeList.txt: cwheel 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.21)

project ("cwheel")
set(CMAKE_CXX_STANDARD 17)
# set(ENV{HTTPS_PROXY}  "http://localhost:1080")

# for clangd
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("src")

# 将源代码添加到此项目的可执行文件。
add_executable (cwheel "cwheel.cpp" "cwheel.h")
set_property(TARGET cwheel PROPERTY CXX_STANDARD 20)

# 单测
add_subdirectory("test")