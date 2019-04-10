cmake_minimum_required(VERSION 3.10)
project(lzhlib
        VERSION 0.3)

set(CMAKE_CXX_STANDARD 17)

set(bin_install_path "bin")
set(main_include_install_path "include/${PROJECT_NAME}-${PROJECT_VERSION}")
set(include_install_path "${main_include_install_path}/${CMAKE_BUILD_TYPE}")
set(lzhlib_include_install_path "${include_install_path}/lzhlib")
set(main_lib_install_path "lib/${PROJECT_NAME}-${PROJECT_VERSION}")
set(lib_install_path "${main_lib_install_path}/${CMAKE_BUILD_TYPE}")

set(header_path "${PROJECT_SOURCE_DIR}/include")

add_library(lzhlib_target INTERFACE)
target_include_directories(lzhlib_target BEFORE INTERFACE
        $<BUILD_INTERFACE:${header_path}>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${include_install_path}>
        )

install(TARGETS lzhlib_target EXPORT lzhlib)
install(EXPORT lzhlib DESTINATION ${lib_install_path})
add_subdirectory("include/${PROJECT_NAME}")
add_subdirectory(src)
add_subdirectory(test)
