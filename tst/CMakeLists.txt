if(NOT CMAKE_CROSSCOMPILING)

    add_executable(testrunner
        Bitmap_test.cpp
        BitmapReader_test.cpp
        BitmapWriter_test.cpp
        BlockCodec_test.cpp
        BlockIterator_test.cpp
        ColorDepth_test.cpp
        DirectDrawSurface_test.cpp
        DirectDrawSurfaceCodec_test.cpp
        DirectDrawSurfaceReader_test.cpp
        DirectDrawSurfaceWriter_test.cpp
        DirectX_test.cpp
        HighColor_test.cpp
        Pipeable_test.cpp
        Texel_test.cpp
        Time_test.cpp
        TransformIterator_test.cpp
        TrueColor_test.cpp)
    target_compile_options(testrunner PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            -Wno-unused-const-variable>)
    target_link_libraries(testrunner PRIVATE img gmock_main)
    add_dependencies(check testrunner)

    add_test(TestRunner testrunner)
    add_coverage(testrunner)

endif()
