/*
 * BitmapWriter_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapWriter.hpp"
#include <gtest/gtest.h>
#include "Writer.hpp"

using namespace image;
using namespace testing;

namespace
{
    inline uint8_t blue(uint32_t color)
    {
        return ((color >> 16) & 0xff);
    }

    inline uint8_t green(uint32_t color)
    {
        return ((color >> 8) & 0xff);
    }

    inline uint8_t red(uint32_t color)
    {
        return (color & 0xff);
    }

    auto breakIntoSamples(uint32_t color)
    {
        return std::make_tuple(blue(color), green(color), red(color));
    }

    auto makeTuple(const Bitmap & bmp)
    {
        const std::tuple<uint8_t, uint8_t> padding;

        return std::tuple_cat(
            std::make_tuple(
                bmp.fileHeader.type[0],
                bmp.fileHeader.type[1],
                bmp.fileHeader.size,
                bmp.fileHeader.reserved,
                bmp.fileHeader.offset),
            std::make_tuple(
                bmp.infoHeader.size,
                bmp.infoHeader.width,
                bmp.infoHeader.height,
                bmp.infoHeader.planes,
                bmp.infoHeader.bits,
                bmp.infoHeader.compression,
                bmp.infoHeader.imageSize,
                bmp.infoHeader.horizontalResolution,
                bmp.infoHeader.verticalResolution,
                bmp.infoHeader.colors,
                bmp.infoHeader.importantColors),
            breakIntoSamples(bmp.colors.at(0)), breakIntoSamples(bmp.colors.at(1)), padding,
            breakIntoSamples(bmp.colors.at(2)), breakIntoSamples(bmp.colors.at(3)), padding);
    }

    class BitmapWriterTest : public Test
    {
    protected:
        std::ostringstream makeBadFile() const
        {
            std::ostringstream file;
            file.setstate(std::ios::badbit);
            return file;
        }

        std::stringstream makeFile(const Bitmap & bmp) const
        {
            std::stringstream file;
            Writer::write(file, makeTuple(bmp));
            return file;
        }

        AssertionResult Equal(std::istream && left, std::istream & right)
        {
            return std::equal(
                std::istreambuf_iterator<char>(left), std::istreambuf_iterator<char>(),
                std::istreambuf_iterator<char>(right)) ?
                    AssertionSuccess() : AssertionFailure();
        }
    };
}

TEST_F(BitmapWriterTest, CanThrowAndCatchBadFile)
{
    try
    {
        throw BitmapWriter::BadFile();
    }
    catch (const std::invalid_argument & e)
    {
        EXPECT_STREQ("bad file", e.what());
    }
}

TEST_F(BitmapWriterTest, GivenBadFile_WhenWriting_ThrowsBadFile)
{
    EXPECT_THROW(BitmapWriter::write(makeBadFile(), Bitmap()), BitmapWriter::BadFile);
}

TEST_F(BitmapWriterTest, CanThrowAndCatchInvalidType)
{
    try
    {
        throw BitmapWriter::InvalidType();
    }
    catch (const std::invalid_argument & e)
    {
        EXPECT_STREQ("invalid type", e.what());
    }
}

TEST_F(BitmapWriterTest, GivenInvalidBitmap_WhenWriting_ThrowsInvalidType)
{
    EXPECT_THROW(BitmapWriter::write(std::ostringstream(), Bitmap()), BitmapWriter::InvalidType);
}

TEST_F(BitmapWriterTest, GivenValidBitmap_WhenWritten_WritesFile)
{
    Bitmap bmp{
        {{ 'B', 'M' }, 70, 1, 54 },
        { 40, 2, 2, 1, 24, 0, 16, 0, 0, 0, 0 },
        { 0xff, 0xffffff, 0xff0000, 0xff00 }};

    std::stringstream file;
    BitmapWriter::write(std::move(file), bmp);
    EXPECT_TRUE(Equal(makeFile(bmp), file));
}
