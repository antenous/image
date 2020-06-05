/*
 * Texel_test.cpp
 *
 *  Created on: Jun 9, 2019
 *      Author: Antero Nousiainen
 */

#include "Texel.hpp"
#include <gtest/gtest.h>

using namespace image;

namespace
{
    TEST(TexelTest, Pixels)
    {
        EXPECT_EQ(16, Texel::pixels());
    }

    TEST(TexelTest, Size)
    {
        EXPECT_EQ(8, Texel::size());
    }

    TEST(TexelTest, Format)
    {
        EXPECT_EQ("{{12, 34}, 0b100111001}", fmt::format("{}", Texel{{12, 34}, 313}));
    }
}
