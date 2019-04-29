/*
 * DirectDrawSurfaceWriter_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceWriter.hpp"
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

    auto toTuple(const DirectDrawSurface::Texel & texel)
    {
        return std::make_tuple(
            texel.referenceColors.first,
            texel.referenceColors.second,
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

    class DirectDrawSurfaceWriterTest : public Test
    {
    protected:
        std::ostringstream makeBadFile() const
        {
            std::ostringstream file;
            file.setstate(std::ios::badbit);
            return file;
        }

        std::stringstream makeFile(const DirectDrawSurface & dds) const
        {
            std::stringstream file;
            Writer::write(file, toTuple(dds));
            return file;
        }

        AssertionResult Equal(std::istream && left, std::istream & right)
        {
            return std::equal(
                std::istreambuf_iterator<char>(left), std::istreambuf_iterator<char>(),
                std::istreambuf_iterator<char>(right)) ?
                    AssertionSuccess() : AssertionFailure();
        }

        DirectDrawSurface dds{
              0x20534444,
            { 124, 0x1 | 0x2 | 0x4 | 0x1000, 4, 4, 8, 0, 0, { 0 },
            { 32, 0x4, 0x31545844, 0, 0, 0, 0, 0 },
              0x1000, 0, 0, 0, 0 },
            {{{ 45316, 54533 }, 1460536927 }}};
    };
}

TEST_F(DirectDrawSurfaceWriterTest, CanThrowAndCatchBadFile)
{
    try
    {
        throw new DirectDrawSurfaceWriter::BadFile();
    }
    catch (const std::invalid_argument * e)
    {
        EXPECT_STREQ("bad file", e->what());
        delete e;
    }
}

TEST_F(DirectDrawSurfaceWriterTest, GivenBadFile_WhenWritten_ThrowsBadFile)
{
    EXPECT_THROW(DirectDrawSurfaceWriter::write(makeBadFile(), dds), DirectDrawSurfaceWriter::BadFile);
}

TEST_F(DirectDrawSurfaceWriterTest, CanThrowAndCatchInvalidType)
{
    try
    {
        throw new DirectDrawSurfaceWriter::InvalidType();
    }
    catch (const std::invalid_argument * e)
    {
        EXPECT_STREQ("invalid type", e->what());
        delete e;
    }
}

TEST_F(DirectDrawSurfaceWriterTest, GivenInvalidDirectDrawSurface_WhenWritten_ThrowsInvalidType)
{
    EXPECT_THROW(DirectDrawSurfaceWriter::write(std::ostringstream(), DirectDrawSurface()), DirectDrawSurfaceWriter::InvalidType);
}

TEST_F(DirectDrawSurfaceWriterTest, GivenValidDirectDrawSurface_WhenWritten_WritesFile)
{
    std::stringstream file;
    DirectDrawSurfaceWriter::write(std::move(file), dds);
    EXPECT_TRUE(Equal(makeFile(dds), file));
}
