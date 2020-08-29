cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0048 NEW) # https://cmake.org/cmake/help/v3.7/policy/CMP0048.html
project(image VERSION 1.3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Werror -Wextra)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(ccache)
include(colordiagnostics)
include(coverage)
include(defaultbuildtype)
include(docker)
include(fmt)
include(googletest)
include(makecheck)
include(uninstall)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)

enable_testing()
add_subdirectory(tst EXCLUDE_FROM_ALL)
check_coverage(check-coverage EXCLUDES '*fmt*' '*googletest*')

# documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(target "doxygen-doc")
    set(DOXYGEN_EXCLUDE_PATTERNS "fmt*" "googletest*")
    set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${target}")
    set(DOXYGEN_GENERATE_HTML YES)
    
    doxygen_add_docs(${target} ${PROJECT_SOURCE_DIR}
        COMMENT "Create project documentation")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMENT "View ${DOXYGEN_OUTPUT_DIRECTORY}/html/index.html to see the documentation")
endif()

# packaging
set(CPACK_PACKAGE_VENDOR "Antero Nousiainen")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A BMP to DDS converter (and the other way around)")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    /\.git.*
    /build
    /\.c?project
    /\.settings/)
include(CPack)
