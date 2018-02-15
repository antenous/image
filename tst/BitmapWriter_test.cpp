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
    auto toTuple(const Bitmap::FileHeader & fileHeader)
    {
        return std::make_tuple(
            fileHeader.type[0],
            fileHeader.type[1],
            fileHeader.size,
            fileHeader.reserved,
            fileHeader.offset);
    }

    auto toTuple(const Bitmap::InfoHeader & infoHeader)
    {
        return std::make_tuple(
            infoHeader.size,
            infoHeader.width,
            infoHeader.height,
            infoHeader.planes,
            infoHeader.bits,
            infoHeader.compression,
            infoHeader.imageSize,
            infoHeader.horizontalResolution,
            infoHeader.verticalResolution,
            infoHeader.colors,
            infoHeader.importantColors);
    }

    template<std::size_t... I>
    auto toTuple(const Bitmap::Data & data, std::index_sequence<I...>)
    {
        return std::make_tuple(data.at(I)...);
    }

    template<std::size_t N>
    auto toTuple(const Bitmap::Data & data)
    {
        return toTuple(data, std::make_index_sequence<N>());
    }

    auto toTuple(const Bitmap & bmp)
    {
        return std::tuple_cat(
            toTuple(bmp.fileHeader),
            toTuple(bmp.infoHeader),
            toTuple<16>(bmp.data));
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
            Writer::write(file, toTuple(bmp));
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
        throw new BitmapWriter::BadFile();
    }
    catch (const std::invalid_argument * e)
    {
        EXPECT_STREQ("bad file", e->what());
        delete e;
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
        throw new BitmapWriter::InvalidType();
    }
    catch (const std::invalid_argument * e)
    {
        EXPECT_STREQ("invalid type", e->what());
        delete e;
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
        { 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
          0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00 }};

    std::stringstream file;
    BitmapWriter::write(std::move(file), bmp);
    EXPECT_TRUE(Equal(makeFile(bmp), file));
}
