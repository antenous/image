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
        const std::vector<uint16_t> original{{
            50005, 54321, 54517, 54533,
            47585, 50001, 45317, 45333,
            47585, 54321, 45317, 45333,
            50005, 50001, 54517, 54533 }};

        const std::vector<uint32_t> compressed{{
            54533U << 16 | 45333,
            0b00101011 << 24 |
            0b01111011 << 16 |
            0b01111011 <<  8 |
            0b00101011 <<  0 }};

        const std::vector<uint32_t> compressedAlpha{{
            45317U << 16 | 54533,
            0b01010111 << 24 |
            0b00001110 << 16 |
            0b00000110 <<  8 |
            0b01011111 <<  0 }};

        const std::vector<uint16_t> uncompressed{{
            47695, 50090, 50090, 54533,
            47695, 50090, 47695, 45333,
            47695, 50090, 47695, 45333,
            47695, 50090, 50090, 54533 }};

        const std::vector<uint16_t> uncompressedAlpha{{
            65535, 54533, 54533, 54533,
            49925, 65535, 45317, 45317,
            49925, 54533, 45317, 45317,
            65535, 65535, 54533, 54533 }};
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
    EXPECT_THROW(BlockCompressor::compress(std::vector<uint16_t>()), BlockCompressor::BadSize);
    EXPECT_THROW(BlockCompressor::compress(std::vector<uint16_t>(1)), BlockCompressor::BadSize);
}

TEST_F(BlockCompressorTest, Compress)
{
    EXPECT_EQ(compressed, BlockCompressor::compress(original));
}

TEST_F(BlockCompressorTest, GivenRangeSizeNotMultipleOfTwo_WhenDecompressed_ThrowsBadSize)
{
    EXPECT_THROW(BlockCompressor::decompress(std::vector<uint32_t>()), BlockCompressor::BadSize);
    EXPECT_THROW(BlockCompressor::decompress(std::vector<uint32_t>(1)), BlockCompressor::BadSize);
}

TEST_F(BlockCompressorTest, Decompress)
{
    EXPECT_EQ(uncompressed, BlockCompressor::decompress(compressed));
}

TEST_F(BlockCompressorTest, DecompressAlpha)
{
    EXPECT_EQ(uncompressedAlpha, BlockCompressor::decompress(compressedAlpha));
}
