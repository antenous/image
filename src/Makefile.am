LIBS = $(CODE_COVERAGE_LIBS)
AM_CPPFLAGS = -I$(top_srcdir)/include $(CODE_COVERAGE_CPPFLAGS)
AM_CXXFLAGS += $(CODE_COVERAGE_CXXFLAGS)

noinst_LTLIBRARIES = libimage.la
libimage_la_SOURCES = \
	../include/Bitmap.hpp \
	../include/BitmapDataReader.hpp \
	../include/BitmapReader.hpp \
	../include/BitmapWriter.hpp \
	../include/BlockCompressor.hpp \
	../include/ColorDepth.hpp \
	../include/ColorPalette.hpp \
	../include/DirectDrawSurface.hpp \
	../include/DirectDrawSurfaceReader.hpp \
	../include/DirectDrawSurfaceWriter.hpp \
	../include/doxygen.hpp \
	../include/ImageConverter.hpp \
	Bitmap.cpp \
	BitmapDataReader.cpp \
	BitmapReader.cpp \
	BitmapWriter.cpp \
	BlockCompressor.cpp \
	ColorDepth.cpp \
	ColorPalette.cpp \
	DirectDrawSurface.cpp \
	DirectDrawSurfaceReader.cpp \
	DirectDrawSurfaceWriter.cpp \
	ImageConverter.cpp

bin_PROGRAMS = image
image_SOURCES = main.cpp
image_LDADD = libimage.la
