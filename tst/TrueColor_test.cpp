/*
 * TrueColor_test.cpp
 *
 *  Created on: Dec 30, 2018
 *      Author: Antero Nousiainen
 */

#include "TrueColor.hpp"
#include <gtest/gtest.h>
#include <sstream>

using namespace image;

namespace
{
    TEST(TrueColorTets, Bits)
    {
        EXPECT_EQ(24, TrueColor::bits);
    }

    TEST(TrueColorTest, Print)
    {
        std::ostringstream oss;
        oss << TrueColor{1, 2, 3};
        EXPECT_EQ("{0x1, 0x2, 0x3}", oss.str());
    }
}
