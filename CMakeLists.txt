cmake_minimum_required(VERSION 3.10)
project(lzhlib
        VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

set(bin_install_path "bin")
set(include_install_path "include/${PROJECT_NAME}")
set(lib_install_path "lib/${PROJECT_NAME}")

set(header_path "${PROJECT_SOURCE_DIR}/include")

include_directories(before "${header_path}")
include_directories(before "${PROJECT_BINARY_DIR}/include")

add_subdirectory("include/${PROJECT_NAME}")
add_subdirectory(test)
add_subdirectory(example)
