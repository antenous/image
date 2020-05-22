add_library(img STATIC
    Bitmap.cpp
    BitmapReader.cpp
    BitmapWriter.cpp
    ColorDepth.cpp
    DirectDrawSurface.cpp
    DirectDrawSurfaceCodec.cpp
    DirectDrawSurfaceReader.cpp
    DirectDrawSurfaceWriter.cpp
    Texel.cpp)
target_include_directories(img PUBLIC ${INCLUDE_DIR})
target_link_libraries(img PUBLIC fmt)
add_coverage(img)

add_executable(image main.cpp)
target_link_libraries(image img)

install(TARGETS image DESTINATION bin CONFIGURATIONS Release)
