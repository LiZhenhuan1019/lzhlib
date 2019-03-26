include(header.cmake)
add_custom_target(verify_lzhlib_utility_headers ${utility_headers})

set(utility_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(utility_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(utility_VERSION ${utility_VERSION_MAJOR}.${utility_VERSION_MINOR} CACHE STRING "version" FORCE)
set(config_input "${PROJECT_SOURCE_DIR}/include/lzhlib/utility/utility_conf.hpp.in")
set(config_output "${PROJECT_BINARY_DIR}/include/lzhlib/utility/utility_conf.hpp")
configure_file(
        ${config_input}
        ${config_output}
)

install(FILES ${utility_headers}
        DESTINATION ${lzhlib_include_install_path}/utility
        COMPONENT utility
        )