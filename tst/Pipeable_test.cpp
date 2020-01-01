/*
 * Pipeable_test.cpp
 *
 *  Created on: Dec 31, 2019
 *      Author: Antero Nousiainen
 */

#include "Pipeable.hpp"
#include <gtest/gtest.h>

using namespace image;

namespace
{
    TEST(PipeableTest, HandleMultipleTypes)
    {
        Pipeable duplicated{
            [](int i){ return i*2; },
            [](const std::string & s){ return s + s; }};

        EXPECT_EQ(6, 3 | duplicated);
        EXPECT_EQ("foofoo", "foo" | duplicated);
    }
}
