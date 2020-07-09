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

# Set the possible build type values for cmake-gui
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# Set the default build type if none is specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE ${default_build_type})
endif()
