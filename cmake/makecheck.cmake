# https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/EmulateMakeCheck

add_custom_target(check COMMAND GTEST_COLOR=yes ${CMAKE_CTEST_COMMAND} --output-on-failure)
