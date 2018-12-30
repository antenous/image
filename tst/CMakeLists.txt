add_executable(testrunner
    BitmapReader_test.cpp
    Bitmap_test.cpp
    BitmapWriter_test.cpp
    BlockCompressor_test.cpp
    BlockIterator_test.cpp
    ColorDepth_test.cpp
    DirectDrawSurfaceReader_test.cpp
    DirectDrawSurface_test.cpp
    DirectDrawSurfaceWriter_test.cpp
    HighColor_test.cpp
    ImageConverter_test.cpp
    TrueColor_test.cpp)
target_link_libraries(testrunner img gtest_main)

add_test(TestRunner testrunner)
add_coverage(testrunner)
