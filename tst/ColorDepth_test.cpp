/*
 * ColorDepth_test.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorDepth.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace image;
using namespace testing;

TEST( ColorDepthTest, Convert24bitTrueColorTo16bitHighColor )
{
    const uint8_t blue(  0b00010000 );
    const uint8_t green( 0b00000100 );
    const uint8_t red(   0b00001000 );

    const uint32_t trueColor( blue << 16 | green << 8 | red );
    const uint16_t highColor( red >> 3 << 11 | green >> 2 << 5 | blue >> 3 );

    EXPECT_EQ( highColor, ColorDepth::trueToHigh( trueColor ));
}

TEST( ColorDepthTest, Convert24bitRangeTo16bitRange )
{
    std::vector< uint32_t > colors{ 0xcafe };
    std::vector< uint16_t > expected{ 0xfe40 };

    EXPECT_EQ( expected, ColorDepth::trueToHigh( colors ));
}

TEST( ColorDepthTest, Convert16bitHighColorTo24BitTrueColor )
{
    const uint8_t blue(  0b11011 );
    const uint8_t green( 0b111101 );
    const uint8_t red(   0b11101 );

    const uint16_t highColor( red << 11 | green << 5 | blue );

    const uint8_t trueBlue(( blue << 3 )|( blue >> 2 ));
    const uint8_t trueGreen(( green << 2 )|( green >> 4 ));
    const uint8_t trueRed(( red << 3 )|( red >> 2 ));

    const uint32_t trueColor( trueBlue << 16  | trueGreen << 8 | trueRed );

    EXPECT_EQ( trueColor, ColorDepth::highToTrue( highColor ));
}

TEST( ColorDepthTest, Convert16bitRangeTo24bitRange )
{
    std::vector< uint16_t > colors{ 0xcafe };
    std::vector< uint32_t > expected{ 0xf75dce };

    EXPECT_EQ( expected, ColorDepth::highToTrue( colors ));
}
