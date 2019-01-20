/*
 * BlockCompressor_test.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#include "BlockCompressor.hpp"
#include <bitset>
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BlockCompressorTest : public Test
    {
    protected:
        const std::vector<TrueColor> original{{
            {0xad, 0x69, 0xc6}, {0x8c, 0x86, 0xd6}, {0xad, 0x9e, 0xd6}, {0x29, 0xa2, 0xd6},
            {0x08, 0x3c, 0xbd}, {0x8c, 0x69, 0xc6}, {0x29, 0x20, 0xb5}, {0xad, 0x20, 0xb5},
            {0x08, 0x3c, 0xbd}, {0x8c, 0x86, 0xd6}, {0x29, 0x20, 0xb5}, {0xad, 0x20, 0xb5},
            {0xad, 0x69, 0xc6}, {0x8c, 0x69, 0xc6}, {0xad, 0x9e, 0xd6}, {0x29, 0xa2, 0xd6} }};

        const DirectDrawSurface::Data compressed{{
            { 54533U, 45333 },
            0b00101011 << 24 |
            0b01111011 << 16 |
            0b01111011 <<  8 |
            0b00101011 <<  0 }};

        const DirectDrawSurface::Data compressedAlpha{{
            { 45317U, 54533 },
            0b01010111 << 24 |
            0b00001110 << 16 |
            0b00000110 <<  8 |
            0b01011111 <<  0 }};

        const std::vector<TrueColor> uncompressed{{
            {0x7b, 0x49, 0xbd}, {0x52, 0x75, 0xc6}, {0x52, 0x75, 0xc6}, {0x29, 0xa2, 0xd6},
            {0x7b, 0x49, 0xbd}, {0x52, 0x75, 0xc6}, {0x7b, 0x49, 0xbd}, {0xad, 0x20, 0xb5},
            {0x7b, 0x49, 0xbd}, {0x52, 0x75, 0xc6}, {0x7b, 0x49, 0xbd}, {0xad, 0x20, 0xb5},
            {0x7b, 0x49, 0xbd}, {0x52, 0x75, 0xc6}, {0x52, 0x75, 0xc6}, {0x29, 0xa2, 0xd6} }};

        const std::vector<TrueColor> uncompressedAlpha{{
            {0x00, 0x00, 0x00}, {0x29, 0xa2, 0xd6}, {0x29, 0xa2, 0xd6}, {0x29, 0xa2, 0xd6},
            {0x29, 0x61, 0xc6}, {0x00, 0x00, 0x00}, {0x29, 0x20, 0xb5}, {0x29, 0x20, 0xb5},
            {0x29, 0x61, 0xc6}, {0x29, 0xa2, 0xd6}, {0x29, 0x20, 0xb5}, {0x29, 0x20, 0xb5},
            {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x29, 0xa2, 0xd6}, {0x29, 0xa2, 0xd6} }};
    };
}

TEST_F(BlockCompressorTest, CanThrowAndCatchBadSize)
{
    try
    {
        throw new BlockCompressor::BadSize();
    }
    catch (const std::invalid_argument * e)
    {
        EXPECT_STREQ("bad size", e->what());
        delete e;
    }
}

TEST_F(BlockCompressorTest, GivenRangeSizeNotMultipleOfSixteen_WhenCompressed_ThrowsBadSize)
{
    EXPECT_THROW(BlockCompressor::compress(std::vector<TrueColor>(), 0, 0), BlockCompressor::BadSize);
    EXPECT_THROW(BlockCompressor::compress(std::vector<TrueColor>(1), 0, 1), BlockCompressor::BadSize);
}

TEST_F(BlockCompressorTest, Compress)
{
    EXPECT_EQ(compressed, BlockCompressor::compress(original, 4, 4));
}

TEST_F(BlockCompressorTest, GivenRangeIsEmpty_WhenDecompressed_ThrowsBadSize)
{
    EXPECT_THROW(BlockCompressor::decompress({ }, 0, 0), BlockCompressor::BadSize);
}

TEST_F(BlockCompressorTest, Decompress)
{
    EXPECT_EQ(uncompressed, BlockCompressor::decompress(compressed, 4, 4));
}

TEST_F(BlockCompressorTest, DecompressAlpha)
{
    EXPECT_EQ(uncompressedAlpha, BlockCompressor::decompress(compressedAlpha, 4, 4));
}
