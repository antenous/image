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
        DirectDrawSurface dds{ };
    };
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
    EXPECT_FALSE(dds);
}

TEST_F(DirectDrawSurfaceTest, Height)
{
    const auto height(dds.header.height = 4);

    EXPECT_EQ(height, dds.height());
}

TEST_F(DirectDrawSurfaceTest, Width)
{
    const auto width(dds.header.width = 4);

    EXPECT_EQ(width, dds.width());
}
