include(header.cmake)

add_custom_target(verify_lzhlib_list_headers SOURCES ${list_headers})
install(FILES ${list_headers}
        DESTINATION ${lzhlib_include_install_path}/list
        COMPONENT list)