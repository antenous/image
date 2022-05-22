# $ eval "$(MSYS_NO_PATHCONV=1 cmd "/C "C:/Program Files \(x86\)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvarsall.bat" x86 > nul && bash -c 'export -p'")"
# $ cmake -S . -B build/msvc -G Ninja -DCMAKE_TOOLCHAIN_FILE=${PWD}/Toolchain-msvc.cmake 

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR $ENV{VSCMD_ARG_TGT_ARCH})
set(CMAKE_SYSTEM_VERSION 10.0)

if(NOT DEFINED ENV{VCToolsInstallDir})
	message(FATAL_ERROR "Please run vcvarsall.bat first")
endif()

file(TO_CMAKE_PATH $ENV{VCToolsInstallDir} VCToolsInstallDir)
file(TO_CMAKE_PATH $ENV{WindowsSdkVerBinPath} WindowsSdkVerBinPath)

# Microsoft MSVC compiler
set(CMAKE_CXX_COMPILER ${VCToolsInstallDir}/bin/Host$ENV{VSCMD_ARG_HOST_ARCH}/$ENV{VSCMD_ARG_TGT_ARCH}/cl.exe)
set(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER})
set(CMAKE_RC_COMPILER ${WindowsSdkVerBinPath}/$ENV{VSCMD_ARG_TGT_ARCH}/rc.exe)
set(CMAKE_MT ${WindowsSdkVerBinPath}/$ENV{VSCMD_ARG_TGT_ARCH}/mt.exe)

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES $ENV{INCLUDE})
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES})

# Workaround since there is no "CMAKE_<LANG>_STANDARD_LINK_DIRECTORIES"
# https://gitlab.kitware.com/cmake/cmake/-/issues/18222
link_directories($ENV{LIB})

# Use ccache to speed up compilations, if available
find_program(CCACHE ccache)
if(CCACHE)
	message(STATUS "ccache found: ${CCACHE}")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()
