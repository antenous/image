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
    };
}

TEST_F(DirectDrawSurfaceTest, FalseWhenEmpty)
{
    EXPECT_FALSE(DirectDrawSurface());
}

TEST_F(DirectDrawSurfaceTest, Height)
{
    DirectDrawSurface dds{ 0 };
    const auto height(dds.header.height = 4);

    EXPECT_EQ(height, dds.height());
}

TEST_F(DirectDrawSurfaceTest, Width)
{
    DirectDrawSurface dds{ 0 };
    const auto width(dds.header.width = 4);

    EXPECT_EQ(width, dds.width());
}
