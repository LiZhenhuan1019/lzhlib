set(config_input "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}_conf.hpp.in")
set(config_output "${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}_conf.hpp")
configure_file(
        ${config_input}
        ${config_output}
)
install(FILES ${config_output} DESTINATION ${include_install_path})

add_subdirectory(graph)
add_subdirectory(repository)
add_subdirectory(stdlib_ext)