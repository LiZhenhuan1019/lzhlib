include(${header_path}/lzhlib/repository/header.cmake)

#message(STATUS "repository_headers = ${repository_headers}")
add_executable(test_repository
        ${repository_headers}
        test_repository.hpp
        test_repository.cpp
        )
target_link_libraries(test_repository lzhlib)
