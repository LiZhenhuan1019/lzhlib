cmake_minimum_required(VERSION 3.10)
project(example1)
find_package(lzhlib)
add_executable(example1 EXCLUDE_FROM_ALL example1.cpp)

target_link_libraries(example1 lzhlib_target)
