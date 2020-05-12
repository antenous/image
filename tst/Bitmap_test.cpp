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
        const std::int32_t height = 2;
        const std::int32_t width = 2;
        const Bitmap::Colors colors{
            {{ 0x00, 0x00, 0xff }, { 0xff, 0xff, 0xff },
             { 0xff, 0x00, 0x00 }, { 0x00, 0xff, 0x00 }}};
        const Bitmap bmp{Bitmap::make(height, width, colors)};
    };

    TEST_F(BitmapTest, Make)
    {
        EXPECT_EQ(0x4d42, bmp.magic);
        EXPECT_EQ(70, bmp.fileHeader.size);
        EXPECT_EQ(54, bmp.fileHeader.offset);

        EXPECT_EQ(40, bmp.infoHeader.size);
        EXPECT_EQ(width, bmp.infoHeader.width);
        EXPECT_EQ(height, bmp.infoHeader.height);
        EXPECT_EQ(1, bmp.infoHeader.planes);
        EXPECT_EQ(24, bmp.infoHeader.bits);
        EXPECT_EQ(0, bmp.infoHeader.compression);
        EXPECT_EQ(16, bmp.infoHeader.imageSize);
        EXPECT_EQ(2834, bmp.infoHeader.horizontalResolution);
        EXPECT_EQ(2834, bmp.infoHeader.verticalResolution);
        EXPECT_EQ(0, bmp.infoHeader.colors);
        EXPECT_EQ(0, bmp.infoHeader.importantColors);

        EXPECT_EQ(colors, bmp.colors);
    }

    TEST_F(BitmapTest, FalseIfEmpty)
    {
        EXPECT_FALSE(Bitmap());
    }

    TEST_F(BitmapTest, Height)
    {
        EXPECT_EQ(height, bmp.height());
    }

    TEST_F(BitmapTest, Width)
    {
        EXPECT_EQ(width, bmp.width());
    }
}
