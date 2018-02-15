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
    const std::vector<ColorDepth::TrueColor> colors{ 0xcafe };
    const std::vector<ColorDepth::HighColor> expected{ 0xfe40 };

    EXPECT_EQ(expected, ColorDepth::trueToHigh(colors));
}

TEST(ColorDepthTest, Convert16bitRangeTo24bitRange)
{
    const std::vector<ColorDepth::HighColor> colors{ 0xcafe };
    const std::vector<ColorDepth::TrueColor> expected{ 0xf75dce };

    EXPECT_EQ(expected, ColorDepth::highToTrue(colors));
}
