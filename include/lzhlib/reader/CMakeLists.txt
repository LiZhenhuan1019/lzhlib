include(header.cmake)
add_custom_target(verify_reader_headers ${reader_headers})

install(FILES ${reader_headers}
        DESTINATION ${include_install_path}/reader
        COMPONENT reader
        )