include(${header_path}/lzhlib/reader/header.cmake)
add_executable(test_reader
        ${reader_headers}
        test_reader.cpp
        test_reader.hpp
        test_literal_reader.hpp
        test_dynamic_reader.hpp
        )
target_link_libraries(test_reader lzhlib)