cmake_minimum_required(VERSION 3.10)
project(lzhlib
        VERSION 0.4)

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif ()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LZHLIB_BUILD_GTEST "Build GoogleTest based unit tests" OFF)
option(LZHLIB_BUILD_BENCHMARKS "Build Google Benchmark based micro benchmarks" OFF)

set(bin_install_path "bin")
set(main_include_install_path "include/${PROJECT_NAME}-${PROJECT_VERSION}")
set(include_install_path "${main_include_install_path}/${CMAKE_BUILD_TYPE}")
set(lzhlib_include_install_path "${include_install_path}/lzhlib")
set(main_lib_install_path "lib/${PROJECT_NAME}-${PROJECT_VERSION}")
set(lib_install_path "${main_lib_install_path}/${CMAKE_BUILD_TYPE}")

set(header_path "${PROJECT_SOURCE_DIR}/include")

add_library(lzhlib INTERFACE)
target_include_directories(lzhlib BEFORE INTERFACE
        $<BUILD_INTERFACE:${header_path}>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${include_install_path}>
        )

install(TARGETS lzhlib EXPORT lzhlib_target)
install(EXPORT lzhlib_target DESTINATION ${lib_install_path})
add_subdirectory("include/${PROJECT_NAME}")
add_subdirectory(src)

# ctest has to be enabled in the top level directory, otherwise the add_test()
# calls in test/ are silently ignored.
enable_testing()
add_subdirectory(test)

if (LZHLIB_BUILD_GTEST)
    add_subdirectory(test/unit)
endif ()

if (LZHLIB_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif ()
