/*
 * BitmapConverter_test.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "BitmapConverter.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include "MockBitmap.hpp"

using namespace image;
using namespace testing;

namespace
{
    class DetailsTester : public BitmapConverter
    {
    public:
        using BitmapConverter::trueToHigh;

        using BitmapConverter::highToTrue;

        using BitmapConverter::rearrangePaletteToBlocks;

        using BitmapConverter::createColorTable;

        using BitmapConverter::createLookupTable;
    };

    class BitmapConverterTest : public Test
    {
    protected:
        void convertBlockToPalette()
        {
            palette.insert( palette.begin(), block.begin(), block.end() );
            std::transform( palette.begin(), palette.end(), palette.begin(),
                [&]( int16_t color ){ return details.highToTrue( color ); });
        }

        BitmapConverter converter;
        DetailsTester details;
        MockBitmap bitmap;
        MockBitmap::Palette palette;

        const BitmapConverter::Block block{{
            131, 191, 251, 313,
            282, 281, 162, 161,
            221, 313, 131, 222,
            250, 132, 312, 190
        }};

        const uint32_t expected{
            ( 0 << 6 | 2 << 4 | 3 << 2 | 1 ) << 24 |
            ( 1 << 6 | 3 << 4 | 2 << 2 | 0 ) << 16 |
            ( 2 << 6 | 1 << 4 | 0 << 2 | 3 ) <<  8 |
            ( 3 << 6 | 0 << 4 | 1 << 2 | 2 )};

        const uint32_t referenceColors{ 131 << 16 | 313 };
    };
}

TEST_F( BitmapConverterTest, CanThrowAndCatchBadSize )
{
    try
    {
        throw BitmapConverter::BadSize();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad size", e.what() );
    }
}

TEST_F( BitmapConverterTest, GivenHeightIsNotMultipleOfFour_WhenConverted_ThrowsBadSize )
{
    EXPECT_CALL( bitmap, getPalette() ).WillOnce( Return( palette ));
    EXPECT_CALL( bitmap, getHeight() ).WillOnce( Return( 3 ));
    EXPECT_CALL( bitmap, getWidth() ).WillOnce( Return( 4 ));

    EXPECT_THROW( converter.convert( bitmap ), BitmapConverter::BadSize );
}

TEST_F( BitmapConverterTest, GivenWidthIsNotMultipleOfFour_WhenConverted_ThrowsBadSize )
{
    EXPECT_CALL( bitmap, getPalette() ).WillOnce( Return( palette ));
    EXPECT_CALL( bitmap, getHeight() ).WillOnce( Return( 4 ));
    EXPECT_CALL( bitmap, getWidth() ).WillOnce( Return( 5 ));

    EXPECT_THROW( converter.convert( bitmap ), BitmapConverter::BadSize );
}

TEST_F( BitmapConverterTest, GivenPaletteSizeIsNotMultipleOfSixteen_WhenConverted_ThrowsBadSize )
{
    palette.resize( 15 );
    EXPECT_CALL( bitmap, getPalette() ).WillOnce( Return( palette ));
    EXPECT_CALL( bitmap, getHeight() ).WillOnce( Return( 4 ));
    EXPECT_CALL( bitmap, getWidth() ).WillOnce( Return( 4 ));

    EXPECT_THROW( converter.convert( bitmap ), BitmapConverter::BadSize );
}

TEST_F( BitmapConverterTest, ConvertBitmap )
{
    convertBlockToPalette();

    EXPECT_CALL( bitmap, getPalette() ).WillOnce( Return( palette ));
    EXPECT_CALL( bitmap, getHeight() ).WillOnce( Return( 4 ));
    EXPECT_CALL( bitmap, getWidth() ).WillOnce( Return( 4 ));

    const auto result( converter.convert( bitmap ));

    ASSERT_FALSE( result.empty() );
    EXPECT_EQ( referenceColors, result[0] );
    EXPECT_EQ( expected, result[1] );
}

TEST_F( BitmapConverterTest, Convert24bitTrueColorTo16bitHighColor )
{
    const uint8_t blue(  0b00010000 );
    const uint8_t green( 0b00000100 );
    const uint8_t red(   0b00001000 );

    const uint32_t trueColor( blue << 16 | green << 8 | red );
    const uint16_t highColor( red >> 3 << 11 | green >> 2 << 5 | blue >> 3 );

    EXPECT_EQ( highColor, details.trueToHigh( trueColor ));
}

TEST_F( BitmapConverterTest, Convert16bitHighColorTo24BitTrueColor )
{
    const uint8_t blue(  0b11011 );
    const uint8_t green( 0b111101 );
    const uint8_t red(   0b11101 );

    const uint16_t highColor( red << 11 | green << 5 | blue );
    const uint32_t trueColor( blue << ( 16 + 3 ) | green << ( 8 + 2 ) | red << 3 );

    EXPECT_EQ( trueColor, details.highToTrue( highColor ));
}

TEST_F( BitmapConverterTest, RearrangePaletteToBlocks )
{
    const BitmapConverter::Palette palette{
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 };

    const BitmapConverter::Palette expected{
        32, 33, 34, 35, 40, 41, 42, 43, 48, 49, 50, 51, 56, 57, 58, 59,
        36, 37, 38, 39, 44, 45, 46, 47, 52, 53, 54, 55, 60, 61, 62, 63,
         0,  1,  2,  3,  8,  9, 10, 11, 16, 17, 18, 19, 24, 25, 26, 27,
         4,  5,  6,  7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31 };

    EXPECT_EQ( expected, details.rearrangePaletteToBlocks( 8, 8, palette ));
}

TEST_F( BitmapConverterTest, CreateColorTable )
{
    auto color( details.createColorTable( block ));

    EXPECT_EQ( 4, color.size() );
    EXPECT_EQ( 131, color[0] );
    EXPECT_EQ( 313, color[1] );
    EXPECT_EQ( 191, color[2] );
    EXPECT_EQ( 251, color[3] );
}

TEST_F( BitmapConverterTest, CreateLookupTable )
{
    const auto color( details.createColorTable( block ));
    EXPECT_EQ( expected, details.createLookupTable( color, block ));
}
