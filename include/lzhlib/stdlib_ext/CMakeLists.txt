include(header.cmake)
add_custom_target(verify_stdlib_ext_headers ${stdlib_ext_headers})

set(stdlib_ext_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(stdlib_ext_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(stdlib_ext_VERSION ${stdlib_ext_VERSION_MAJOR}.${stdlib_ext_VERSION_MINOR} CACHE STRING "version" FORCE)
set(config_input "${PROJECT_SOURCE_DIR}/include/lzhlib/stdlib_ext/stdlib_ext_conf.hpp.in")
set(config_output "${PROJECT_BINARY_DIR}/include/lzhlib/stdlib_ext/stdlib_ext_conf.hpp")
configure_file(
        ${config_input}
        ${config_output}
)

install(FILES ${stdlib_ext_headers}
        DESTINATION ${include_install_path}/stdlib_ext
        COMPONENT stdlib_ext
        )