/*
 * ColorPalette_test.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorPalette.hpp"
#include <vector>
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class ColorPaletteTest : public Test
    {
    protected:
        const std::vector< HighColor > bmp{
             0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
            32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
            48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 };

        const std::vector< HighColor > dds{
            32, 33, 34, 35, 40, 41, 42, 43, 48, 49, 50, 51, 56, 57, 58, 59,
            36, 37, 38, 39, 44, 45, 46, 47, 52, 53, 54, 55, 60, 61, 62, 63,
             0,  1,  2,  3,  8,  9, 10, 11, 16, 17, 18, 19, 24, 25, 26, 27,
             4,  5,  6,  7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31 };
    };
}

TEST_F( ColorPaletteTest, CanThrowAndCatchBadDimensions )
{
    try
    {
        throw new ColorPalette::BadDimensions();
    }
    catch ( const std::runtime_error * e )
    {
        EXPECT_STREQ( "bad dimensions", e->what() );
        delete e;
    }
}

TEST_F( ColorPaletteTest, GivenHeightIsNotMultipleOfFour_WhenRearranged_ThrowsBadDimensions )
{
    EXPECT_THROW( ColorPalette::rearrangeForBitmap( 0, 4, dds ), ColorPalette::BadDimensions );
    EXPECT_THROW( ColorPalette::rearrangeForDirectDrawSurface( 0, 4, bmp ), ColorPalette::BadDimensions );
}

TEST_F( ColorPaletteTest, GivenWidthIsNotMultipleOfFour_WhenRearranged_ThrowsBadDimensions )
{
    EXPECT_THROW( ColorPalette::rearrangeForBitmap( 4, 0, dds ), ColorPalette::BadDimensions );
    EXPECT_THROW( ColorPalette::rearrangeForDirectDrawSurface( 4, 0, bmp ), ColorPalette::BadDimensions );
}

TEST_F( ColorPaletteTest, GivenColorPaletteIsNotMultipleOfSixteen_WhenRearranged_ThrowsBadDimensions )
{
    EXPECT_THROW( ColorPalette::rearrangeForBitmap( 4, 4, { }), ColorPalette::BadDimensions );
    EXPECT_THROW( ColorPalette::rearrangeForDirectDrawSurface( 4, 4, { }), ColorPalette::BadDimensions );
}

TEST_F( ColorPaletteTest, RearrangeColorPaletteForBitmap )
{
    EXPECT_EQ( bmp, ColorPalette::rearrangeForBitmap( 8, 8, dds ));
}

TEST_F( ColorPaletteTest, RearrangeColorPaletteForDirectDrawSurface )
{
    EXPECT_EQ( dds, ColorPalette::rearrangeForDirectDrawSurface( 8, 8, bmp ));
}
