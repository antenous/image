cmake_minimum_required(VERSION 3.12)
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
include(makecheck)
include(uninstall)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)

enable_testing()
add_subdirectory(tst EXCLUDE_FROM_ALL)
check_coverage(check-coverage EXCLUDES '*googletest*')

# documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(target "doxygen-doc")
    set(DOXYGEN_EXCLUDE_PATTERNS "googletest*")
    set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${target}")
    set(DOXYGEN_GENERATE_HTML YES)
    
    doxygen_add_docs(${target} ${PROJECT_SOURCE_DIR}
        COMMENT "Create project documentation")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMENT "View ${DOXYGEN_OUTPUT_DIRECTORY}/html/index.html to see the documentation")
endif()
