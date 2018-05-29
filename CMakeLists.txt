cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW) # https://cmake.org/cmake/help/v3.7/policy/CMP0048.html
project(image VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Werror -Wextra -Wno-missing-field-initializers")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(coverage)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)

enable_testing()
add_subdirectory(tst EXCLUDE_FROM_ALL)
add_subdirectory(3rdparty/googletest EXCLUDE_FROM_ALL)

# https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/EmulateMakeCheck
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_dependencies(check image testrunner)

add_code_coverage_target(check-code-coverage EXCLUDES
    '${PROJECT_SOURCE_DIR}/3rdparty/*'
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
    # TODO: Take `doxygen_add_docs` into use (added in CMake 3.9)

    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(GENERATE_HTML "YES")
    set(GENERATE_HTMLHELP "NO")
    set(GENERATE_CHI "NO")
    set(GENERATE_LATEX "YES")
    set(PAPER_SIZE "a4")
    set(GENERATE_RTF "NO")
    set(GENERATE_MAN "NO")
    set(GENERATE_XML "NO")

    find_program(DOT dot)
    if(DOT)
        set(HAVE_DOT "YES")
    else()
        set(HAVE_DOT "NO")
    endif()

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    set(target "doxygen-doc")

    add_custom_target(${target}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM)

    add_custom_command(TARGET ${target} POST_BUILD
        COMMENT "View ${PROJECT_BINARY_DIR}/${target}/html/index.html to see the documentation")
endif()
