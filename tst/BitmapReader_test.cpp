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
            Writer::write(file, toTuple(bmp));
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
        PrintTo(toTuple(bmp), os);
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
    Bitmap invalid{
        {{ 'M', 'B' }, 70, 1, 54 },
        { 40, 2, 2, 1, 24, 0, 16, 0, 0, 0, 0 },
        { 'b', 'l', 'u', 'e', 'g', 'r', '\0', '\0',
          'e', 'e', 'n', 'r', 'e', 'd', '\0', '\0' }};

    EXPECT_THROW(BitmapReader::read(makeFile(invalid)), BitmapReader::InvalidType);
}

TEST_F(BitmapReaderTest, GivenValidFile_WhenRead_CreatesBitmap)
{
    Bitmap bmp{
        {{ 'B', 'M' }, 70, 1, 54 },
        { 40, 2, 2, 1, 24, 0, 16, 0, 0, 0, 0 },
        { 'b', 'l', 'u', 'e', 'g', 'r', '\0', '\0',
          'e', 'e', 'n', 'r', 'e', 'd', '\0', '\0' }};

    EXPECT_EQ(bmp, BitmapReader::read(makeFile(bmp)));
}
