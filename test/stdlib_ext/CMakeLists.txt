include(${header_path}/lzhlib/stdlib_ext/header.cmake)

add_executable(test_stdlib_ext
        ${stdlib_ext_headers}
        test_stdlib_ext.cpp
        test_tuple_ext.hpp
        test_stdlib_ext.hpp)
target_link_libraries(test_stdlib_ext lzhlib_target)