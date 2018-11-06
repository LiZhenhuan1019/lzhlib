include(${header_path}/lzhlib/graph/header.cmake)
include(${header_path}/lzhlib/repository/header.cmake)

#message(STATUS "graph_headers = ${graph_headers}")
#message(STATUS "repository_headers = ${repository_headers}")
add_executable(test_graph
        ${graph_headers}
        ${repository_headers}
        test_graph.cpp
        white_box_test.cpp
        white_box_test.hpp )
target_link_libraries(test_graph lzhlib_target)