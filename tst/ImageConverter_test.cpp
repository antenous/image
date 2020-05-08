/*
 * ImageConverter_test.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "ImageConverter.hpp"
#include <gtest/gtest.h>
#include "BitmapWriter.hpp"
#include "DirectDrawSurfaceWriter.hpp"
#include "DirectX.hpp"

using namespace image;
using namespace testing;

namespace
{
    class ImageConverterTest : public Test
    {
    protected:
        std::string toString(const Bitmap & bmp) const
        {
            std::stringstream file;
            BitmapWriter::write(std::move(file), bmp);
            return file.str();
        }

        std::string toString(const DirectDrawSurface & dds) const
        {
            std::stringstream file;
            DirectDrawSurfaceWriter::write(std::move(file), dds);
            return file.str();
        }

        const Bitmap bmp{Bitmap::make(4, 4,
            {{ 0x18, 0x10, 0x00 }, { 0x52, 0x14, 0x00 }, { 0x8c, 0x1c, 0x00 }, { 0xce, 0x24, 0x00 },
             { 0xce, 0x24, 0x00 }, { 0xce, 0x24, 0x00 }, { 0x52, 0x14, 0x00 }, { 0x52, 0x14, 0x00 },
             { 0x8c, 0x1c, 0x00 }, { 0xce, 0x24, 0x00 }, { 0x18, 0x10, 0x00 }, { 0x8c, 0x1c, 0x00 },
             { 0x8c, 0x1c, 0x00 }, { 0x18, 0x10, 0x00 }, { 0xce, 0x24, 0x00 }, { 0x52, 0x14, 0x00 }})};

        DirectDrawSurface dds{
             DirectX::DDS_MAGIC,
            {124, DirectX::DDS_HEADER_FLAGS_TEXTURE, 4, 4, 8, 0, 0, {},
            {32, DirectX::DDS_FOURCC, DirectX::DDS_DXT1, 0, 0, 0, 0, 0},
             DirectX::DDS_SURFACE_FLAGS_TEXTURE, 0, 0, 0, 0},
            {{{ 313, 131 }, 770740934 }}};
    };
}

TEST_F(ImageConverterTest, CanThrowAndCatchBadBitmap)
{
    try
    {
        throw new ImageConverter::BadBitmap();
    }
    catch (const std::runtime_error * e)
    {
        EXPECT_STREQ("bad bitmap", e->what());
        delete e;
    }
}

TEST_F(ImageConverterTest, GivenBadBitmap_WhenConverted_ThrowsBadBitmap)
{
    EXPECT_THROW(ImageConverter::convert(Bitmap()), ImageConverter::BadBitmap);
}

TEST_F(ImageConverterTest, ConvertBitmap)
{
    EXPECT_EQ(toString(dds), toString(ImageConverter::convert(bmp)));
}

TEST_F(ImageConverterTest, CanThrowAndCatchBadDirectDrawSurface)
{
    try
    {
        throw new ImageConverter::BadDirectDrawSurface();
    }
    catch (const std::runtime_error * e)
    {
        EXPECT_STREQ("bad direct draw surface", e->what());
        delete e;
    }
}

TEST_F(ImageConverterTest, GivenBadDirectDrawSurface_WhenConverted_ThrowsBadDirectDrawSurface)
{
    EXPECT_THROW(ImageConverter::convert(DirectDrawSurface()), ImageConverter::BadDirectDrawSurface);
}

TEST_F(ImageConverterTest, ConvertDirectDrawSurface)
{
    EXPECT_EQ(toString(bmp), toString(ImageConverter::convert(dds)));
}
