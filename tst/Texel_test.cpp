/*
 * Texel_test.cpp
 *
 *  Created on: Jun 9, 2019
 *      Author: Antero Nousiainen
 */

#include "Texel.hpp"
#include <gtest/gtest.h>

using namespace image;

TEST(TexelTest, Pixels)
{
    EXPECT_EQ(16, Texel::pixels());
}

TEST(TexelTest, Size)
{
    EXPECT_EQ(8, Texel::size());
}
