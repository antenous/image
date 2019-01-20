/*
 * HighColor_test.cpp
 *
 *  Created on: Jan 12, 2019
 *      Author: Antero Nousiainen
 */

#include "HighColor.hpp"
#include <gtest/gtest.h>

using namespace image;

TEST(HighColorTest, Type)
{
    EXPECT_TRUE((std::is_same<uint16_t, HighColor::Color>::value));
}
