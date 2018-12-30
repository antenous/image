/*
 * ColorDepth_test.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorDepth.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

TEST(ColorDepthTest, Convert24bitColorTo16bit)
{
    const TrueColor color{ 'r', 'e', 'd' };
    const HighColor expected{ 0x632e };

    EXPECT_EQ(expected, ColorDepth::trueToHigh(color));
}

TEST(ColorDepthTest, Convert16bitColorTo24bit)
{
    const HighColor color{ 0xcafe };
    const TrueColor expected{ 0xf7, 0x5d, 0xce };

    EXPECT_EQ(expected, ColorDepth::highToTrue(color));
}
