# Add a custom target to create a code coverage report:
#
#   include(coverage)
#   add_code_coverage_target(<make target>)
#
# For every test target added with `add_test` add the following:
#
#   add_coverage(<test target>)
#
# To create a code coverage report run the below commands:
#
# $ cmake -H. -Bbuild -DENABLE_CODE_COVERAGE=ON
# $ cmake --build build/ --target check-code-coverage

option(ENABLE_CODE_COVERAGE "Enable code coverage" OFF)

# Stubs when the coverage is not in use
function(add_coverage test)
endfunction()

function(add_code_coverage_target target)
endfunction()

# No point going further if coverage is not in use
if(NOT ENABLE_CODE_COVERAGE)
    return()
endif ()

# Enforce a Debug build
if(NOT CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE EQUAL Debug)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_program(LCOV lcov)
find_program(GENHTML genhtml)

if(NOT LCOV)
    message(FATAL_ERROR "Unable to find lcov")
endif()

if(NOT GENHTML)
    message(FATAL_ERROR "Unable to find genhtml")
endif()

# Set coverage flags
string(APPEND CMAKE_CXX_FLAGS " -O0 --coverage -fprofile-arcs -ftest-coverage")

# Add a test target dependency to the code coverage generation
set(Coverage_TARGETS "" CACHE INTERNAL "")
function(add_coverage target)
    set(Coverage_TARGETS ${Coverage_TARGETS} ${target} CACHE INTERNAL "")
endfunction()

# code coverage target
# https://wiki.documentfoundation.org/Development/Lcov
function(add_code_coverage_target target)

    set(options)
    set(oneValueArgs)
    set(multiValueArgs EXCLUDES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(report ${PROJECT_NAME}-coverage)
    set(excludes '/usr/include/*' ${Coverage_EXCLUDES})

    add_custom_target(${target}
        COMMENT "Creating code coverage report"

        # Cleanup and run baseline lcov
        COMMAND ${LCOV} --zerocounters --directory . --quiet
        COMMAND ${LCOV} --capture --initial --directory . --output-file ${report}.base --quiet

        # Run the tests
        COMMAND ${CMAKE_CTEST_COMMAND}

        # Run lcov again after tests
        COMMAND ${LCOV} --capture --directory . --output-file ${report}.info --quiet

        # Combine lcov tracefiles
        COMMAND ${LCOV} --add-tracefile ${report}.base --add-tracefile ${report}.info --output-file ${report}.total --quiet

        # Remove unwanted stuff from tracefile
        COMMAND ${LCOV} --remove ${report}.total ${excludes} --output-file ${report}.info.cleaned --quiet

        # Generate HTML reports.
        COMMAND ${GENHTML} --title ${PROJECT_NAME} --output-directory=${report} ${report}.info.cleaned --quiet

        # Cleanup
        COMMAND ${CMAKE_COMMAND} -E remove ${report}.base ${report}.total ${report}.info.cleaned
    )

    add_dependencies(${target} ${Coverage_TARGETS})

    add_custom_command(TARGET ${target} POST_BUILD
        COMMENT "View ${PROJECT_BINARY_DIR}/${report}/index.html to see the report")

endfunction()
