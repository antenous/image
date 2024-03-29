/*
 * DirectDrawSurfaceReader_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceReader.hpp"
#include <gtest/gtest.h>
#include "Writer.hpp"

using namespace image;
using namespace testing;

namespace
{
    auto toTuple(uint32_t magic)
    {
        return std::make_tuple(magic);
    }

    template<std::size_t N, typename Array, std::size_t... I>
    auto toTuple(Array array[N], std::integer_sequence<std::size_t, I...>)
    {
        return std::make_tuple(array[I]...);
    }

    template<std::size_t N, typename Array>
    auto toTuple(Array array[N])
    {
        return toTuple<N>(array, std::make_integer_sequence<std::size_t, N>());
    }

    auto toTuple(const DirectDrawSurface::Header::PixelFormat & pixelFormat)
    {
        return std::make_tuple(
            pixelFormat.size,
            pixelFormat.flags,
            pixelFormat.fourCC,
            pixelFormat.bits,
            pixelFormat.redBitMask,
            pixelFormat.greenBitMask,
            pixelFormat.blueBitMask,
            pixelFormat.alphaBitMask);
    }

    auto toTuple(const DirectDrawSurface::Header & header)
    {
        return std::tuple_cat(
            std::make_tuple(
                header.size,
                header.flags,
                header.height,
                header.width,
                header.pitch,
                header.depth,
                header.mipmaps),
            toTuple<11>(header.reserved1),
            toTuple(header.pixelFormat),
            std::make_tuple(
                header.caps,
                header.caps2,
                header.caps3,
                header.caps4,
                header.reserved2));
    }

    auto toTuple(const Texel & texel)
    {
        return std::make_tuple(
            texel.referenceColors[0],
            texel.referenceColors[1],
            texel.lookupTable);
    }

    auto toTuple(const DirectDrawSurface::Data & data)
    {
        return toTuple(data.at(0));
    }

    auto toTuple(const DirectDrawSurface & dds)
    {
        return std::tuple_cat(
            toTuple(dds.magic),
            toTuple(dds.header),
            toTuple(dds.data));
    }

    std::stringstream makeFile(const DirectDrawSurface & dds)
    {
        std::stringstream file;
        Writer::write(file, toTuple(dds));
        return file;
    }

    class DirectDrawSurfaceReaderTest : public Test
    {
    protected:
        void SetUp() override
        {
            badFile.setstate(std::ios::badbit);
        }

        std::istringstream badFile;
        std::istringstream emptyFile;

        DirectDrawSurface dds{DirectDrawSurface::make(4, 4,
            {{{0xffffU, 0x1fU},
               0b00000000U << 24 |
               0b00000000U << 16 |
               0b00000101U << 8  |
               0b00000101U}})};
    };
}

namespace image
{
    void PrintTo(const DirectDrawSurface & dds, std::ostream * os)
    {
        *os << fmt::format("{}, {}, {}", dds.magic, toTuple(dds.header), dds.data);
    }

    bool operator==(const DirectDrawSurface & lhs, const DirectDrawSurface & rhs)
    {
        return PrintToString(lhs) == PrintToString(rhs);
    }
}
TEST_F(DirectDrawSurfaceReaderTest, CanThrowAndCatchBadFile)
{
    try
    {
        throw new DirectDrawSurfaceReader::BadFile();
    }
    catch (const std::invalid_argument * e)
    {
        EXPECT_STREQ("bad file", e->what());
        delete e;
    }
}

TEST_F(DirectDrawSurfaceReaderTest, GivenBadFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(DirectDrawSurfaceReader::read(badFile), DirectDrawSurfaceReader::BadFile);
}

TEST_F(DirectDrawSurfaceReaderTest, GivenEmptyFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(DirectDrawSurfaceReader::read(emptyFile), DirectDrawSurfaceReader::BadFile);
}

TEST_F(DirectDrawSurfaceReaderTest, CanThrowAndCatchInvalidType)
{
    try
    {
        throw new DirectDrawSurfaceReader::InvalidType();
    }
    catch (const std::runtime_error * e)
    {
        EXPECT_STREQ("invalid type", e->what());
        delete e;
    }
}

TEST_F(DirectDrawSurfaceReaderTest, GivenFileWithInvalidMagic_WhenRead_ThrowsInvalidType)
{
    dds.magic = 313;
    auto fileWithInvalidMagic(makeFile(dds));

    EXPECT_THROW(DirectDrawSurfaceReader::read(fileWithInvalidMagic), DirectDrawSurfaceReader::InvalidType);
}

TEST_F(DirectDrawSurfaceReaderTest, GivenValidFile_WhenRead_CreatesDirectDrawSurface)
{
    auto validFile(makeFile(dds));
    EXPECT_EQ(dds, DirectDrawSurfaceReader::read(validFile));
}
