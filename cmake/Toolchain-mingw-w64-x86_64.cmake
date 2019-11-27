set(CMAKE_SYSTEM_NAME Windows)
set(TRIPLET x86_64-w64-mingw32)

# Cross compilers for C/C++
set(CMAKE_C_COMPILER ${TRIPLET}-gcc)
set(CMAKE_CXX_COMPILER ${TRIPLET}-g++)
set(CMAKE_RC_COMPILER ${TRIPLET}-windres)

# Target environment on the build host
set(CMAKE_FIND_ROOT_PATH /usr/${TRIPLET})

# Search headers and libraries in the target environment,
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_EXE_LINKER_FLAGS_INIT "-static" CACHE STRING "Linker flags")
