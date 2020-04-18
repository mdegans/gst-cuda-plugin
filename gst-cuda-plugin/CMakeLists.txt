find_package(PkgConfig REQUIRED)

add_subdirectory(config_h)

# gstreamer stuff
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-base-1.0)
include_directories(${GSTREAMER_INCLUDE_DIRS})

# dsfilter stuff
pkg_check_modules(DSFILTER REQUIRED dsfilter)
include_directories(${DSFILTER_INCLUDE_DIRS})
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

# local source and targets
include_directories(include)
include_directories(config_h)
add_library(gstcudaplugin SHARED
    src/gstcudafilter.cpp
    src/gstcudaplugin.cpp
)

target_link_libraries(
    gstcudaplugin ${GSTREAMER_LIBRARIES} ${DSFILTER_LIBRARIES}
)

install(TARGETS gstcudaplugin
    LIBRARY
        DESTINATION lib/x86_64-linux-gnu/gstreamer-1.0/
)

# TODO(mdegans): cmake pkg-config generation