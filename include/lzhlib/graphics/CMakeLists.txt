include(header.cmake)
add_custom_target(verify_lzhlib_graphics_headers SOURCES ${graph_headers})
install(FILES ${graphics_headers}
        DESTINATION ${lzhlib_include_install_path}/graphics
        COMPONENT graphics
        )
