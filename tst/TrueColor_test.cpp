/*
 * TrueColor_test.cpp
 *
 *  Created on: Dec 30, 2018
 *      Author: Antero Nousiainen
 */

#include "TrueColor.hpp"
#include <gtest/gtest.h>

using namespace image;

namespace
{
    TEST(TrueColorTets, Bits)
    {
        EXPECT_EQ(24, TrueColor::bits);
    }

    TEST(TrueColorTest, Format)
    {
        EXPECT_EQ("{0x01, 0x02, 0x03}", fmt::format("{}", TrueColor{1, 2, 3}));
    }
}
