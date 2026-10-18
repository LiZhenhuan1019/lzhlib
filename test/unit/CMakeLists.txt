# Unit tests based on GoogleTest.
#
# Prefer a system installed GoogleTest (faster, works offline), otherwise fetch it.
find_package(GTest QUIET)
if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()

add_executable(lzhlib_unit_tests
        matrix_test.cpp
        indexed_list_test.cpp
        value_repo_test.cpp
        thread_test.cpp)

target_link_libraries(lzhlib_unit_tests PRIVATE lzhlib GTest::gtest_main)

include(GoogleTest)
# PRE_TEST defers the test discovery to ctest time instead of running the freshly
# built (and possibly sanitizer instrumented) binary as a POST_BUILD step.
gtest_discover_tests(lzhlib_unit_tests DISCOVERY_MODE PRE_TEST)