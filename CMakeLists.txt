cmake_minimum_required(VERSION 3.10)
project(lzhlib)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I./include -DLZHLIB_BUILDING_AND_TESTING")

add_executable(lzhlib
        include/graph/directed_graph.hpp
        include/graph/edge.hpp
        include/graph/edge_id.hpp
        include/graph/graph.hpp
        include/graph/graph_base.hpp
        include/graph/undirected_graph.hpp
        include/graph/vertex.hpp
        include/graph/vertex_id.hpp
        include/repository/dynamic_repository.hpp
        include/repository/indexed_repository.hpp
        include/repository/light_weight_repository.hpp
        include/repository/repository.hpp
        include/repository/sparse_indexed_repository.hpp
        include/repository/stable_repository.hpp
        include/repository/stock_id.hpp
        test/test.cpp
        test/graph/white_box_test.cpp
        test/graph/white_box_test.hpp test/graph/test_graph.hpp test/repository/test_repository.hpp)
