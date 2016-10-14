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
    class DetailsTester : public BitmapConverter
    {
    public:
        using BitmapConverter::createColorTable;

        using BitmapConverter::createLookupTable;

        using BitmapConverter::createColorBlock;
    };

    class BitmapConverterTest : public Test
    {
    protected:
        void convertBlockToPalette( const BitmapConverter::Block & block )
        {
            palette.insert( palette.begin(), block.begin(), block.end() );
            std::transform( palette.begin(), palette.end(), palette.begin(),
                [&]( int16_t color ){ return ColorDepth::highToTrue( color ); });
        }

        BitmapConverter converter;
        DetailsTester details;
        MockBitmap bitmap;
        MockDirectDrawSurface dds;
        MockBitmap::Palette palette;

        const BitmapConverter::Block block{{
            131, 170, 241, 313,
            282, 281, 162, 161,
            221, 313, 131, 222,
            250, 132, 312, 190 }};

        const BitmapConverter::Block generatedBlock{{
            131, 170, 241, 313,
            313, 313, 170, 170,
            241, 313, 131, 241,
            241, 131, 313, 170 }};

        const BitmapConverter::Color color{{ 131, 313, 170, 241 }};

        const uint32_t lookup{
            ( 0 << 6 | 2 << 4 | 3 << 2 | 1 ) << 24 |
            ( 3 << 6 | 3 << 4 | 0 << 2 | 0 ) << 16 |
            ( 2 << 6 | 1 << 4 | 0 << 2 | 2 ) <<  8 |
            ( 3 << 6 | 0 << 4 | 1 << 2 | 2 )};

        const uint32_t referenceColors{ 131 << 16 | 313 };

        DirectDrawSurface::Surface surface{ referenceColors, lookup };
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

TEST_F( BitmapConverterTest, ConvertBitmap )
{
    convertBlockToPalette( block );

    EXPECT_CALL( bitmap, getPalette() ).WillOnce( Return( palette ));
    EXPECT_CALL( bitmap, getHeight() ).WillOnce( Return( 4 ));
    EXPECT_CALL( bitmap, getWidth() ).WillOnce( Return( 4 ));

    EXPECT_EQ( surface, converter.convert( bitmap ));
}

TEST_F( BitmapConverterTest, GivenDirectDrawSurfaceIsNotMultipleOfTwo_WhenConverted_ThrowsBadSize )
{
    surface.clear();
    EXPECT_CALL( dds, getSurface() ).WillOnce( Return( surface ));

    EXPECT_THROW( converter.convert( dds ), BitmapConverter::BadSize );
}

TEST_F( BitmapConverterTest, ConvertDirectDrawSurface )
{
    convertBlockToPalette( generatedBlock );

    EXPECT_CALL( dds, getSurface() ).WillOnce( Return( surface ));
    EXPECT_CALL( dds, getHeight() ).WillOnce( Return( 4 ));
    EXPECT_CALL( dds, getWidth() ).WillOnce( Return( 4 ));

    EXPECT_EQ( palette, converter.convert( dds ));
}

TEST_F( BitmapConverterTest, CreateColorTable )
{
    EXPECT_EQ( color, details.createColorTable( block ));
}

TEST_F( BitmapConverterTest, ReCreateColorTable )
{
    EXPECT_EQ( color, details.createColorTable( referenceColors ));
}

TEST_F( BitmapConverterTest, CreateLookupTable )
{
    EXPECT_EQ( lookup, details.createLookupTable( color, block ));
}

TEST_F( BitmapConverterTest, CreateColorBlock )
{
    EXPECT_EQ( generatedBlock, details.createColorBlock( color, lookup ));
}
