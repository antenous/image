/*
 * BitmapReader_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapReader.hpp"
#include <gtest/gtest.h>
#include "TuplePrinter.hpp"
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

    auto componentise(uint32_t color)
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
            componentise(bmp.colors.at(0)), componentise(bmp.colors.at(1)), padding,
            componentise(bmp.colors.at(2)), componentise(bmp.colors.at(3)), padding);
    }

    class BitmapReaderTest : public Test
    {
    protected:
        std::istringstream makeBadFile() const
        {
            std::istringstream file;
            file.setstate(std::ios::badbit);
            return file;
        }

        std::istringstream makeEmptyFile() const
        {
            return std::istringstream();
        }

        std::stringstream makeFile(const Bitmap & bmp) const
        {
            std::stringstream file;
            Writer::write(file, makeTuple(bmp));
            return file;
        }
    };
}

namespace image
{
// TODO: With C++17 we could do this (http://en.cppreference.com/w/cpp/language/fold):
//    template<class Tuple, std::size_t... I>
//    void printTuple(std::ostream * os, const Tuple & t, std::index_sequence<I...>)
//    {
//        ((*os << (I == 0? "" : ", ") << std::get<I>(t)), ...);
//    }
//
//    template<typename... Args>
//    void PrintTo(std::tuple<Args...> && t, std::ostream * os)
//    {
//        printTuple(os, t, std::index_sequence_for<Args...>{});
//    }

    template<typename... Args>
    void PrintTo(std::tuple<Args...> && t, std::ostream * os)
    {
        TuplePrinter<decltype(t), sizeof...(Args)>::print(t, os);
    }

    void PrintTo(const Bitmap & bmp, std::ostream * os)
    {
        PrintTo(makeTuple(bmp), os);
    }

    bool operator==(const Bitmap & lhs, const Bitmap & rhs)
    {
        return makeTuple(lhs) == makeTuple(rhs);
    }
}

TEST_F(BitmapReaderTest, CanThrowAndCatchBadFile)
{
    try
    {
        throw BitmapReader::BadFile();
    }
    catch (const std::invalid_argument & e)
    {
        EXPECT_STREQ("bad file", e.what());
    }
}

TEST_F(BitmapReaderTest, GivenBadFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(BitmapReader::read(makeBadFile()), BitmapReader::BadFile);
}

TEST_F(BitmapReaderTest, GivenEmptyFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(BitmapReader::read(makeEmptyFile()), BitmapReader::BadFile);
}

TEST_F(BitmapReaderTest, CanThrowAndCatchInvalidType)
{
    try
    {
        throw BitmapReader::InvalidType();
    }
    catch (const std::runtime_error & e)
    {
        EXPECT_STREQ("invalid type", e.what());
    }
}

TEST_F(BitmapReaderTest, GivenFileWithInvalidType_WhenRead_ThrowsInvalidType)
{
    Bitmap invalid{
        {{ 'M', 'B' }, 70, 1, 54 },
        { 40, 2, 2, 1, 24, 0, 16, 0, 0, 0, 0 },
        { 0xff, 0xffffff, 0xff0000, 0xff00 }};

    EXPECT_THROW(BitmapReader::read(makeFile(invalid)), BitmapReader::InvalidType);
}

TEST_F(BitmapReaderTest, GivenValidFile_WhenRead_CreatesBitmap)
{
    Bitmap bmp{
        {{ 'B', 'M' }, 70, 1, 54 },
        { 40, 2, 2, 1, 24, 0, 16, 0, 0, 0, 0 },
        { 0xff, 0xffffff, 0xff0000, 0xff00 }};

    EXPECT_EQ(bmp, BitmapReader::read(makeFile(bmp)));
}
