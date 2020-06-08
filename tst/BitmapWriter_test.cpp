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
    using Data = std::vector<uint8_t>;

    auto toTuple(const Bitmap::Header::File & fileHeader)
    {
        return std::make_tuple(
            fileHeader.size,
            fileHeader.reserved,
            fileHeader.offset);
    }

    auto toTuple(const Bitmap::Header::Info & infoHeader)
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
    auto toTuple(const Data & data, std::index_sequence<I...>)
    {
        return std::make_tuple(data.at(I)...);
    }

    template<std::size_t N>
    auto toTuple(const Data & data)
    {
        return toTuple(data, std::make_index_sequence<N>());
    }

    inline void addPadding(Data & data, uint8_t padding)
    {
        data.insert(data.end(), padding, 0);
    }

    auto colorsToData(const Bitmap & bmp)
    {
        Data data;
        data.reserve(bmp.header.info.imageSize);

        for (auto it(bmp.data.begin()); it != bmp.data.end(); addPadding(data, bmp.padding()))
            for (auto end(std::next(it, bmp.width())); it != end; ++it)
                data.insert(data.end(), { it->blue, it->green, it->red });

        return data;
    }

    auto toTuple(const Bitmap & bmp)
    {
        return std::tuple_cat(
            std::make_tuple(bmp.magic),
            toTuple(bmp.header.file),
            toTuple(bmp.header.info),
            toTuple<16>(colorsToData(bmp)));
    }

    class BitmapWriterTest : public Test
    {
    protected:
        void SetUp() override
        {
            badFile.setstate(std::ios::badbit);
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

        std::ostringstream badFile;
        std::stringstream file;

        const Bitmap bmp{ Bitmap::make(2, 2,
            {{ 0x00, 0x00, 0xff }, { 0xff, 0xff, 0xff },
             { 0xff, 0x00, 0x00 }, { 0x00, 0xff, 0x00 }}) };
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

TEST_F(BitmapWriterTest, GivenBadFile_WhenWritten_ThrowsBadFile)
{
    EXPECT_THROW(BitmapWriter::write(badFile, bmp), BitmapWriter::BadFile);
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

TEST_F(BitmapWriterTest, GivenInvalidBitmap_WhenWritten_ThrowsInvalidType)
{
    EXPECT_THROW(BitmapWriter::write(file, Bitmap{ }), BitmapWriter::InvalidType);
}

TEST_F(BitmapWriterTest, GivenValidBitmap_WhenWritten_WritesFile)
{
    BitmapWriter::write(file, bmp);
    EXPECT_TRUE(Equal(makeFile(bmp), file));
}
