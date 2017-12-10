#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define([image_major_version], [1])
m4_define([image_minor_version], [0])
m4_define([image_micro_version], [0])
m4_define([image_version],
          [image_major_version.image_minor_version.image_micro_version])

AC_PREREQ([2.69])
AC_INIT([image], [image_version])

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign -Wall -Werror -Wno-extra-portability subdir-objects 1.13 tar-pax])

# Support silent build rules. Disable by either passing
# --disable-silent-rules to configure or passing
# V=1 to make
AM_SILENT_RULES([yes])

LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_MAKE_SET

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

AX_COMPILER_VERSION
CXX_VERSION=$ax_cv_cxx_compiler_version

AX_CXX_COMPILE_STDCXX(14, [noext], [mandatory])
AX_PTHREAD

# AC_PROG_CXX will by default set 'CXXFLAGS=-g -02'. For the code
# coverage to work the default optimization level will need to be
# removed.
AX_CODE_COVERAGE
AS_IF([ test "$enable_code_coverage" = "yes" ], [
    CXXFLAGS=${CXXFLAGS/-O?/}
])

# Doxygen support
DX_DOT_FEATURE(ON)
DX_INIT_DOXYGEN([$PACKAGE_NAME], [Doxyfile], [doxygen-doc])
AC_CONFIG_FILES([Doxyfile])

AM_CXXFLAGS="-Wall -Werror -Wextra"
AC_SUBST(AM_CXXFLAGS)

# Silence warning: ar: 'u' modifier ignored since 'D' is the default
AC_SUBST(AR_FLAGS, [cr])

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tst/Makefile])
AC_OUTPUT
AC_MSG_RESULT([
        $PACKAGE $VERSION

        prefix:                 ${prefix}
        bindir:                 ${bindir}

        cxx:                    ${CXX}
        cxx version:            ${CXX_VERSION}
        cppflags:               ${CPPFLAGS}
        am_cppflags:            ${AM_CPPFLAGS}
        cxxflags:               ${CXXFLAGS}
        am_cxxflags:            ${AM_CXXFLAGS}
        ldflags:                ${LDFLAGS}
        am_ldflags:             ${AM_LDFLAGS}
])
