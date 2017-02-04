/*
 * BlockCompressor_test.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#include "BlockCompressor.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BlockCompressorTest : public Test
    {
    protected:
        const std::vector< uint16_t > original{{
            131, 170, 241, 313,
            282, 281, 162, 161,
            221, 313, 131, 222,
            250, 132, 312, 190 }};

        const uint32_t referenceColors{ 313 << 16 | 131 };

        const uint32_t lookup{
            ( 0 << 6 | 2 << 4 | 3 << 2 | 1 ) << 24 |
            ( 3 << 6 | 3 << 4 | 0 << 2 | 0 ) << 16 |
            ( 2 << 6 | 1 << 4 | 0 << 2 | 2 ) <<  8 |
            ( 3 << 6 | 0 << 4 | 1 << 2 | 2 )};

        const std::vector< uint32_t > compressed{{ referenceColors, lookup }};

        const std::vector< uint16_t > uncompressed{{
            131, 170, 241, 313,
            313, 313, 170, 170,
            241, 313, 131, 241,
            241, 131, 313, 170 }};
    };
}

TEST_F( BlockCompressorTest, CanThrowAndCatchBadSize )
{
    try
    {
        throw BlockCompressor::BadSize();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad size", e.what() );
    }
}

TEST_F( BlockCompressorTest, GivenRangeSizeNotMultipleOfSixteen_WhenCompressed_ThrowsBadSize )
{
    EXPECT_THROW( BlockCompressor::compress( std::vector< uint16_t >() ), BlockCompressor::BadSize );
    EXPECT_THROW( BlockCompressor::compress( std::vector< uint16_t >( 1 )), BlockCompressor::BadSize );
}

TEST_F( BlockCompressorTest, Compress )
{
    EXPECT_EQ( compressed, BlockCompressor::compress( original ));
}

TEST_F( BlockCompressorTest, GivenRangeSizeNotMultipleOfTwo_WhenDecompressed_ThrowsBadSize )
{
    EXPECT_THROW( BlockCompressor::decompress( std::vector< uint32_t >() ), BlockCompressor::BadSize );
    EXPECT_THROW( BlockCompressor::decompress( std::vector< uint32_t >( 1 )), BlockCompressor::BadSize );
}

TEST_F( BlockCompressorTest, Decompress )
{
    EXPECT_EQ( uncompressed, BlockCompressor::decompress( compressed ));
}
