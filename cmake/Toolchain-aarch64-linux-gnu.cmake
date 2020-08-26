set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(triplet aarch64-linux-gnu)

# Cross compilers for C/C++
set(CMAKE_C_COMPILER ${triplet}-gcc)
set(CMAKE_CXX_COMPILER ${triplet}-g++)

# Target environment on the build host
set(CMAKE_FIND_ROOT_PATH /usr/${triplet})

# Search headers and libraries in the target environment,
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
