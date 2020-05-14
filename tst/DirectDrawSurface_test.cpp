/*
 * DirectDrawSurface_test.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class DirectDrawSurfaceTest : public Test
    {
    protected:
        const std::uint32_t height = 4;
        const std::uint32_t width = 4;
        const DirectDrawSurface::Data data{{{ 0xf00, 0xba2 }, 0xe4e4e4e4 }};
        const DirectDrawSurface dds{ DirectDrawSurface::make(height, width, data) };
    };

    TEST_F(DirectDrawSurfaceTest, Make)
    {
        EXPECT_EQ(DirectX::DDS_MAGIC, dds.magic);

        EXPECT_EQ(124, dds.header.size);
        EXPECT_EQ(DirectX::DDS_HEADER_FLAGS_TEXTURE, dds.header.flags);
        EXPECT_EQ(height, dds.header.height);
        EXPECT_EQ(width, dds.header.width);
        EXPECT_EQ(8, dds.header.pitch);
        EXPECT_EQ(0, dds.header.depth);
        EXPECT_EQ(1, dds.header.mipmaps);

        EXPECT_EQ(32, dds.header.pixelFormat.size);
        EXPECT_EQ(DirectX::DDS_FOURCC, dds.header.pixelFormat.flags);
        EXPECT_EQ(DirectX::DDS_DXT1, dds.header.pixelFormat.fourCC);
        EXPECT_EQ(0, dds.header.pixelFormat.bits);
        EXPECT_EQ(0, dds.header.pixelFormat.redBitMask);
        EXPECT_EQ(0, dds.header.pixelFormat.greenBitMask);
        EXPECT_EQ(0, dds.header.pixelFormat.blueBitMask);
        EXPECT_EQ(0, dds.header.pixelFormat.alphaBitMask);

        EXPECT_EQ(DirectX::DDS_SURFACE_FLAGS_TEXTURE, dds.header.caps);

        EXPECT_EQ(data, dds.data);
    }

    TEST_F(DirectDrawSurfaceTest, SizeOfHeader)
    {
        EXPECT_EQ(124, sizeof(DirectDrawSurface::Header));
    }

    TEST_F(DirectDrawSurfaceTest, SizeOfPixelFormat)
    {
        EXPECT_EQ(32, sizeof(DirectDrawSurface::Header::PixelFormat));
    }

    TEST_F(DirectDrawSurfaceTest, FalseWhenEmpty)
    {
        EXPECT_FALSE(DirectDrawSurface{ });
    }

    TEST_F(DirectDrawSurfaceTest, Height)
    {
        EXPECT_EQ(height, dds.height());
    }

    TEST_F(DirectDrawSurfaceTest, Width)
    {
        EXPECT_EQ(width, dds.width());
    }
}
