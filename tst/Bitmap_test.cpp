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
        const Bitmap::Data data{
            {{ 0x00, 0x00, 0xff }, { 0xff, 0xff, 0xff },
             { 0xff, 0x00, 0x00 }, { 0x00, 0xff, 0x00 }}};
        const Bitmap bmp{Bitmap::make(height, width, data)};
    };

    TEST_F(BitmapTest, Make)
    {
        EXPECT_EQ(0x4d42, bmp.magic);
        EXPECT_EQ(70U, bmp.header.file.size);
        EXPECT_EQ(54U, bmp.header.file.offset);

        EXPECT_EQ(40U, bmp.header.info.size);
        EXPECT_EQ(width, bmp.header.info.width);
        EXPECT_EQ(height, bmp.header.info.height);
        EXPECT_EQ(1U, bmp.header.info.planes);
        EXPECT_EQ(24U, bmp.header.info.bits);
        EXPECT_EQ(0U, bmp.header.info.compression);
        EXPECT_EQ(16U, bmp.header.info.imageSize);
        EXPECT_EQ(2834U, bmp.header.info.horizontalResolution);
        EXPECT_EQ(2834U, bmp.header.info.verticalResolution);
        EXPECT_EQ(0U, bmp.header.info.colors);
        EXPECT_EQ(0U, bmp.header.info.importantColors);

        EXPECT_EQ(data, bmp.data);
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
