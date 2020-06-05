cmake_minimum_required(VERSION 3.14)
include(FetchContent)

FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        6.2.1)

FetchContent_MakeAvailable(fmt)
