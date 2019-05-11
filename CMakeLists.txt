cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0048 NEW) # https://cmake.org/cmake/help/v3.7/policy/CMP0048.html
project(image VERSION 1.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Werror -Wextra")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(ccache)
include(coverage)
include(docker)
include(googletest)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)

enable_testing()
add_subdirectory(tst EXCLUDE_FROM_ALL)

# https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/EmulateMakeCheck
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_dependencies(check image testrunner)

add_code_coverage_target(check-code-coverage EXCLUDES
    '${PROJECT_BINARY_DIR}/googletest*'
    '${PROJECT_SOURCE_DIR}/src/main.cpp')

# uninstall target
# https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# documentation
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(target "doxygen-doc")
    set(DOXYGEN_EXCLUDE_PATTERNS "*/3rdparty/*")
    set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${target}")
    set(DOXYGEN_GENERATE_HTML YES)
    
    doxygen_add_docs(${target} ${PROJECT_SOURCE_DIR}
        COMMENT "Create project documentation")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMENT "View ${DOXYGEN_OUTPUT_DIRECTORY}/html/index.html to see the documentation")
endif()
