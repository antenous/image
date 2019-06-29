add_executable(testrunner
    Bitmap_test.cpp
    BitmapCompressor_test.cpp
    BitmapReader_test.cpp
    BitmapWriter_test.cpp
    BlockCodec_test.cpp
    BlockIterator_test.cpp
    ColorDepth_test.cpp
    DirectDrawSurface_test.cpp
    DirectDrawSurfaceDecompressor_test.cpp
    DirectDrawSurfaceReader_test.cpp
    DirectDrawSurfaceWriter_test.cpp
    DirectX_test.cpp
    HighColor_test.cpp
    ImageConverter_test.cpp
    Texel_test.cpp
    TransformIterator_test.cpp
    TrueColor_test.cpp)
target_link_libraries(testrunner img gtest_main)

add_test(TestRunner testrunner)
add_coverage(testrunner)
