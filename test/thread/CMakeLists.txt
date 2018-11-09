include(${header_path}/lzhlib/thread/header.cmake)

add_executable(test_thread
        ${thread_headers}
        test_thread.cpp
        test_thread.hpp
        test_queue.hpp)
target_link_libraries(test_thread lzhlib_target)