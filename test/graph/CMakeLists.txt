include(${header_path}/lzhlib/graph/header.cmake)
include(${header_path}/lzhlib/repository/header.cmake)

add_executable(test_graph
        ${graph_headers}
        ${repository_headers}
        test_graphics.cpp
        white_box_test.cpp
        white_box_test.hpp )
target_link_libraries(test_graph lzhlib_target)