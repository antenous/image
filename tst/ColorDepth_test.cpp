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

TEST(ColorDepthTest, Convert24bitRangeTo16bitRange)
{
    const std::vector<TrueColor> colors{{ 'r', 'e', 'd' }};
    const std::vector<HighColor> expected{ 0x632e };

    EXPECT_EQ(expected, ColorDepth::trueToHigh(colors));
}

TEST(ColorDepthTest, Convert16bitRangeTo24bitRange)
{
    const std::vector<HighColor> colors{ 0xcafe };
    const std::vector<TrueColor> expected{{ 0xf7, 0x5d, 0xce }};

    EXPECT_EQ(expected, ColorDepth::highToTrue(colors));
}
