include(header.cmake)
add_custom_target(verify_lzhlib_thread_headers ${thread_headers})

install(FILES ${thread_headers}
        DESTINATION ${lzhlib_include_install_path}/thread
        COMPONENT thread
        )