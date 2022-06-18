# The default CMake build type, when none is specified, is
#   "Release" (without optimization flags) for single-config generators (Makefile, Ninja) and
#   "Debug" for multi-config generators (MSBuild, XCode).
#
# This module sets the default CMake build type, when none is specified, to
#   "Debug" for a cloned project and
#   "Release" for a source package.
#
# https://blog.kitware.com/cmake-and-the-default-build-type/

set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type "Debug")
endif()

# Set the default build type if none is specified
# Does not apply with MSVC (indicated by the presence of the CMAKE_CONFIGURATION_TYPES)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Using default build type: '${default_build_type}'")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Build type" FORCE)

    # Set the possible build type values for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
