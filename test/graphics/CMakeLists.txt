include(${header_path}/lzhlib/graphics/header.cmake)

add_executable(test_graphics
        ${graphics_headers}
        test_graphics.cpp)
target_link_libraries(test_graphics lzhlib)