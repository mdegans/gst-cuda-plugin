# https://cmake.org/cmake/help/v3.6/command/configure_file.html
configure_file(config_cmake.h.in config_cmake.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DHAVE_CONFIG_CMAKE_H)