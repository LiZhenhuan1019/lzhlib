include(header.cmake)
add_custom_target(verify_reader_headers ${reader_headers})

install(FILES ${reader_headers}
        DESTINATION ${lzhlib_include_install_path}/reader
        COMPONENT reader
        )