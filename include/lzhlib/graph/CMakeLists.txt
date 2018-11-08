include(header.cmake)
add_custom_target(verify_lzhlib_graph_headers SOURCES ${graph_headers})
install(FILES ${graph_headers}
        DESTINATION ${lzhlib_include_install_path}/graph
        COMPONENT graph
        )
