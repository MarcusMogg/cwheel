# CMakeList.txt: cwheel 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.14)

project ("cwheel")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory("src")

# 将源代码添加到此项目的可执行文件。
add_executable (cwheel "cwheel.cpp" "cwheel.h")

# 单测
add_subdirectory("test")