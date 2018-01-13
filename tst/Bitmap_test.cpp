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

TEST_F(BitmapTest, FalseIfEmpty)
{
    EXPECT_FALSE(Bitmap());
}

TEST_F(BitmapTest, Height)
{
    EXPECT_EQ(2, bmp.height());
}

TEST_F(BitmapTest, Width)
{
    EXPECT_EQ(2, bmp.width());
}

TEST_F(BitmapTest, DataToColors)
{
    bmp.data = data;
    EXPECT_EQ(colors, bmp.colors());
}

TEST_F(BitmapTest, ColorsToData)
{
    bmp.colors(colors);
    EXPECT_EQ(data, bmp.data);
}
