include(header.cmake)
add_custom_target(verify_graph_headers SOURCES ${graph_hearders})
install(FILES ${graph_headers}
        DESTINATION ${include_install_path}/graph
        COMPONENT graph
        )
