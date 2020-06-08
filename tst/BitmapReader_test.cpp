/*
 * BitmapReader_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapReader.hpp"
#include <fmt/ranges.h>
#include <gtest/gtest.h>
#include "DirectX.hpp"
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

    class BitmapReaderTest : public Test
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

        std::istringstream badFile;
        std::istringstream emptyFile;

        Bitmap bmp{ Bitmap::make(2, 2,
            {{ 'b', 'l', 'u' }, { 'e', 'g', 'r' },
             { 'e', 'e', 'n' }, { 'r', 'e', 'd' }}) };
    };
}

namespace image
{
    void PrintTo(const Bitmap & bmp, std::ostream * os)
    {
        *os << fmt::format("{}, {}, {}, {}", bmp.magic, toTuple(bmp.header.file),
            toTuple(bmp.header.info), bmp.data);
    }

    bool operator==(const Bitmap & lhs, const Bitmap & rhs)
    {
        return PrintToString(lhs) == PrintToString(rhs);
    }
}

TEST_F(BitmapReaderTest, CanThrowAndCatchBadFile)
{
    try
    {
        throw new BitmapReader::BadFile();
    }
    catch (const std::invalid_argument * e)
    {
        EXPECT_STREQ("bad file", e->what());
        delete e;
    }
}

TEST_F(BitmapReaderTest, GivenBadFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(BitmapReader::read(badFile), BitmapReader::BadFile);
}

TEST_F(BitmapReaderTest, GivenEmptyFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(BitmapReader::read(emptyFile), BitmapReader::BadFile);
}

TEST_F(BitmapReaderTest, CanThrowAndCatchInvalidType)
{
    try
    {
        throw new BitmapReader::InvalidType();
    }
    catch (const std::runtime_error * e)
    {
        EXPECT_STREQ("invalid type", e->what());
        delete e;
    }
}

TEST_F(BitmapReaderTest, GivenFileWithInvalidType_WhenRead_ThrowsInvalidType)
{
    bmp.magic = DirectX::detail::makeMagic<Bitmap::Magic>({'M','B'});
    auto fileWithInvalidType(makeFile(bmp));
    EXPECT_THROW(BitmapReader::read(fileWithInvalidType), BitmapReader::InvalidType);
}

TEST_F(BitmapReaderTest, GivenValidFile_WhenRead_CreatesBitmap)
{
    auto validFile(makeFile(bmp));
    EXPECT_EQ(bmp, BitmapReader::read(validFile));
}
