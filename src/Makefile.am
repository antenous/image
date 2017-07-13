LIBS = $(CODE_COVERAGE_LIBS)
AM_CPPFLAGS = -I$(top_srcdir)/include $(CODE_COVERAGE_CPPFLAGS)
AM_CXXFLAGS += $(CODE_COVERAGE_CXXFLAGS)

bin_PROGRAMS = image
image_SOURCES = \
	../include/Bitmap.hpp \
	../include/BitmapDataReader.hpp \
	../include/BlockCompressor.hpp \
	../include/ColorDepth.hpp \
	../include/ColorPalette.hpp \
	../include/DirectDrawSurface.hpp \
	../include/doxygen.hpp \
	../include/ImageConverter.hpp \
	Bitmap.cpp \
	BitmapDataReader.cpp \
	BlockCompressor.cpp \
	ColorDepth.cpp \
	ColorPalette.cpp \
	DirectDrawSurface.cpp \
	ImageConverter.cpp \
	main.cpp
