/*
 * BitmapConverter_test.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "BitmapConverter.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include "ColorDepth.hpp"
#include "MockBitmap.hpp"
#include "MockDirectDrawSurface.hpp"

using namespace image;
using namespace testing;

namespace
{
    class BitmapConverterTest : public Test
    {
    protected:
        BitmapConverter converter;
        MockBitmap bitmap;
        MockDirectDrawSurface dds;

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

        std::vector< uint32_t > compressed{ referenceColors, lookup };

        const std::vector< uint16_t > uncompressed{{
            131, 170, 241, 313,
            313, 313, 170, 170,
            241, 313, 131, 241,
            241, 131, 313, 170 }};
    };
}

TEST_F( BitmapConverterTest, ConvertBitmap )
{
    EXPECT_CALL( bitmap, getPalette() ).WillOnce( Return( ColorDepth::highToTrue( original )));
    EXPECT_CALL( bitmap, getHeight() ).WillOnce( Return( 4 ));
    EXPECT_CALL( bitmap, getWidth() ).WillOnce( Return( 4 ));

    EXPECT_EQ( compressed, converter.convert( bitmap ));
}

TEST_F( BitmapConverterTest, ConvertDirectDrawSurface )
{
    EXPECT_CALL( dds, getSurface() ).WillOnce( Return( compressed ));
    EXPECT_CALL( dds, getHeight() ).WillOnce( Return( 4 ));
    EXPECT_CALL( dds, getWidth() ).WillOnce( Return( 4 ));

    EXPECT_EQ( ColorDepth::highToTrue( uncompressed ), converter.convert( dds ));
}
