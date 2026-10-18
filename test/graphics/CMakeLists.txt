include(${header_path}/lzhlib/graphics/header.cmake)

add_executable(test_graphics
        ${graphics_headers}
        test_graphics.cpp)
target_link_libraries(test_graphics lzhlib)
add_test(NAME test_graphics COMMAND test_graphics)

# The legacy test initializes the nested std::array of basic_matrix flat
# (basic_matrix<int, 3, 2>{6, 8, 9, 12, 15, 20}), which is valid C++ and how
# the library is meant to be used, but clang reports it under -Wmissing-braces.
# Rewriting the generated 30x35 literals into nested braces would only obscure
# them, so the warning is disabled for this single target.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(test_graphics PRIVATE -Wno-missing-braces)
endif ()
