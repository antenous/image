/*
 * Bitmap_test.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BitmapTest : public Test
    {
    protected:
        Bitmap bmp{
            {{ 'B', 'M' }, 70, 1, 54 },
            { 40, 2, 2, 1, 24, 0, 16, 0, 0, 0, 0 },
            { }};
        Bitmap::Colors colors{ 0xff, 0xffffff, 0xff0000, 0xff00 };
        Bitmap::Data data{
            0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
            0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00 };
    };
}

TEST_F(BitmapTest, EmptyBitmapEvaluatesToFalse)
{
    EXPECT_FALSE(Bitmap());
}

TEST_F(BitmapTest, GetHeight)
{
    EXPECT_EQ(2, bmp.getHeight());
}

TEST_F(BitmapTest, GetWidth)
{
    EXPECT_EQ(2, bmp.getWidth());
}

TEST_F(BitmapTest, GetColors)
{
    bmp.data = data;
    EXPECT_EQ(colors, bmp.getColors());
}

TEST_F(BitmapTest, SetColors)
{
    bmp.setColors(colors);
    EXPECT_EQ(data, bmp.data);
}
