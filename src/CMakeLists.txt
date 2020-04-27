add_library(img STATIC
    Bitmap.cpp
    BitmapCompressor.cpp
    BitmapReader.cpp
    BitmapWriter.cpp
    ColorDepth.cpp
    DirectDrawSurface.cpp
    DirectDrawSurfaceDecompressor.cpp
    DirectDrawSurfaceReader.cpp
    DirectDrawSurfaceWriter.cpp
    ImageConverter.cpp
    Texel.cpp)
target_include_directories(img PUBLIC ${INCLUDE_DIR})
add_coverage(img)

add_executable(image main.cpp)
target_link_libraries(image img)

install(TARGETS image DESTINATION bin CONFIGURATIONS Release)
