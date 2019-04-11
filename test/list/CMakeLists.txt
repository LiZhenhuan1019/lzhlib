include(${header_path}/lzhlib/list/header.cmake)

add_executable(test_list
        ${list_headers}
        test_indexed_list.hpp
        test_list.cpp
        test_list.hpp)
target_link_libraries(test_list lzhlib)