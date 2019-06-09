/*
 * BlockCodec_test.cpp
 *
 *  Created on: Jun 9, 2019
 *      Author: Antero Nousiainen
 */

#include "BlockCodec.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BlockCodecTest : public Test
    {
    protected:
        const std::array<HighColor, 16> original{
            0xc3a4, 0xd505, 0xd505, 0xd505,
            0xba44, 0xc3a4, 0xb104, 0xb104,
            0xba44, 0xd505, 0xb104, 0xb104,
            0xc3a4, 0xc3a4, 0xd505, 0xd505};

        const Texel compressed{
            {0xd505, 0xb104},
            0b00000010 << 24 |
            0b01011011 << 16 |
            0b01010011 <<  8 |
            0b00001010 <<  0 };

        const Texel compressedAlpha{
            {0xb104, 0xd505},
            0b01010111 << 24 |
            0b00001110 << 16 |
            0b00000110 <<  8 |
            0b01011111 <<  0 };

        const std::array<HighColor, 16> uncompressedAlpha{
            0x0000, 0xd505, 0xd505, 0xd505,
            0xc304, 0x0000, 0xb104, 0xb104,
            0xc304, 0xd505, 0xb104, 0xb104,
            0x0000, 0x0000, 0xd505, 0xd505};
    };
}

TEST_F(BlockCodecTest, Compress)
{
    EXPECT_EQ(compressed, BlockCodec::compress(original.begin(), original.end()));
}

TEST_F(BlockCodecTest, Decompress)
{
    std::array<HighColor, 16> decompressed{};
    EXPECT_EQ(decompressed.end(), BlockCodec::decompress(compressedAlpha, decompressed.begin()));
    EXPECT_EQ(uncompressedAlpha, decompressed);
}
