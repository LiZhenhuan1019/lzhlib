include(header.cmake)

add_custom_target(verify_lzhlib_repository_headers SOURCES ${repository_headers})
install(FILES ${repository_headers}
        DESTINATION ${lzhlib_include_install_path}/repository
        COMPONENT repository)