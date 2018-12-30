add_library(img STATIC
    Bitmap.cpp
    BitmapReader.cpp
    BitmapWriter.cpp
    BlockCompressor.cpp
    ColorDepth.cpp
    DirectDrawSurface.cpp
    DirectDrawSurfaceReader.cpp
    DirectDrawSurfaceWriter.cpp
    ImageConverter.cpp)
target_include_directories(img PUBLIC ${INCLUDE_DIR})

add_executable(image main.cpp)
target_link_libraries(image img)

install(TARGETS image DESTINATION bin CONFIGURATIONS Release)
