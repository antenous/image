AM_LIBS = $(CODE_COVERAGE_LIBS) $(PTHREAD_LIBS)
AM_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS)
AM_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS) $(PTHREAD_CFLAGS)

# Test framework libraries are build only when 'make check' is issued
check_LTLIBRARIES = libgtest.la libgmock.la libgmock_main.la

# Google Test
GTEST_DIR = $(top_srcdir)/3rdparty/googletest/googletest
AM_CPPFLAGS += -isystem $(GTEST_DIR)/include

libgtest_la_SOURCES = \
	../3rdparty/googletest/googletest/include/gtest/gtest.h \
	../3rdparty/googletest/googletest/include/gtest/gtest-death-test.h \
	../3rdparty/googletest/googletest/include/gtest/gtest-message.h \
	../3rdparty/googletest/googletest/include/gtest/gtest-param-test.h \
	../3rdparty/googletest/googletest/include/gtest/gtest_pred_impl.h \
	../3rdparty/googletest/googletest/include/gtest/gtest-printers.h \
	../3rdparty/googletest/googletest/include/gtest/gtest_prod.h \
	../3rdparty/googletest/googletest/include/gtest/gtest-spi.h \
	../3rdparty/googletest/googletest/include/gtest/gtest-test-part.h \
	../3rdparty/googletest/googletest/include/gtest/gtest-typed-test.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-death-test-internal.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-filepath.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-internal.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-linked_ptr.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-param-util.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-param-util-generated.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-port.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-port-arch.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-string.h \
	../3rdparty/googletest/googletest/include/gtest/internal/gtest-type-util.h \
	../3rdparty/googletest/googletest/include/gtest/internal/custom/gtest.h \
	../3rdparty/googletest/googletest/include/gtest/internal/custom/gtest-port.h \
	../3rdparty/googletest/googletest/include/gtest/internal/custom/gtest-printers.h \
	../3rdparty/googletest/googletest/src/gtest-all.cc \
	../3rdparty/googletest/googletest/src/gtest.cc \
	../3rdparty/googletest/googletest/src/gtest-death-test.cc \
	../3rdparty/googletest/googletest/src/gtest-filepath.cc \
	../3rdparty/googletest/googletest/src/gtest-internal-inl.h \
	../3rdparty/googletest/googletest/src/gtest-port.cc \
	../3rdparty/googletest/googletest/src/gtest-printers.cc \
	../3rdparty/googletest/googletest/src/gtest-test-part.cc \
	../3rdparty/googletest/googletest/src/gtest-typed-test.cc
libgtest_la_CPPFLAGS = -I$(GTEST_DIR) $(AM_CPPFLAGS)

# Google Mock
GMOCK_DIR = $(top_srcdir)/3rdparty/googletest/googlemock
AM_CPPFLAGS += -isystem $(GMOCK_DIR)/include

libgmock_la_SOURCES = \
	../3rdparty/googletest/googlemock/src/gmock-all.cc \
	../3rdparty/googletest/googlemock/src/gmock.cc \
	../3rdparty/googletest/googlemock/src/gmock-cardinalities.cc \
	../3rdparty/googletest/googlemock/src/gmock-internal-utils.cc \
	../3rdparty/googletest/googlemock/src/gmock-matchers.cc \
	../3rdparty/googletest/googlemock/src/gmock-spec-builders.cc \
	../3rdparty/googletest/googlemock/include/gmock/gmock.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-actions.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-cardinalities.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-generated-actions.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-generated-function-mockers.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-generated-matchers.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-generated-nice-strict.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-matchers.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-more-actions.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-more-matchers.h \
	../3rdparty/googletest/googlemock/include/gmock/gmock-spec-builders.h \
	../3rdparty/googletest/googlemock/include/gmock/internal/gmock-internal-utils.h \
	../3rdparty/googletest/googlemock/include/gmock/internal/gmock-generated-internal-utils.h \
	../3rdparty/googletest/googlemock/include/gmock/internal/gmock-port.h \
	../3rdparty/googletest/googlemock/include/gmock/internal/custom/gmock-port.h \
	../3rdparty/googletest/googlemock/include/gmock/internal/custom/gmock-generated-actions.h \
	../3rdparty/googletest/googlemock/include/gmock/internal/custom/gmock-matchers.h
libgmock_la_CPPFLAGS = -I$(GMOCK_DIR) $(AM_CPPFLAGS)

# Google Mock main
libgmock_main_la_SOURCES = ../3rdparty/googletest/googlemock/src/gmock_main.cc
libgmock_main_la_LIBADD = libgtest.la libgmock.la

# TESTS -- Programs run automatically by 'make check'
# check_PROGRAMS -- Programs built by 'make check' but not necessarily run
TESTS = testrunner
check_PROGRAMS = testrunner

testrunner_SOURCES = \
	Bitmap_test.cpp \
	BlockCompressor_test.cpp \
	ColorDepth_test.cpp \
	ColorPalette_test.cpp \
	DirectDrawSurface_test.cpp \
	ImageConverter_test.cpp \
	MockBitmap.hpp \
	MockDirectDrawSurface.hpp
testrunner_CPPFLAGS = -I$(top_srcdir)/include $(AM_CPPFLAGS)
testrunner_LDADD = \
	libgmock_main.la \
	../src/Bitmap.o \
	../src/ColorDepth.o \
	../src/DirectDrawSurface.o \
	../src/BlockCompressor.o \
	../src/ColorPalette.o \
	../src/ImageConverter.o
