# test_check.hpp is shared by the hand written tests below.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Every hand written test is registered with ctest, so a plain "ctest" runs
# both the GoogleTest suite (test/unit) and these checks.
add_subdirectory(graph)
add_subdirectory(graphics)
add_subdirectory(list)
add_subdirectory(repository)
add_subdirectory(utility)
add_subdirectory(reader)
add_subdirectory(thread)
