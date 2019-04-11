include(${header_path}/lzhlib/utility/header.cmake)

add_executable(test_utility
        ${utility_headers}
        test_utility.cpp
        test_tuple_ext.hpp
        test_utility.hpp
        test_optional.hpp)
target_link_libraries(test_utility lzhlib)