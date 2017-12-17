/*
 * BitmapReader_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapReader.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BitmapReaderTest : public Test
    {

    };
}

TEST_F(BitmapReaderTest, GivenEmptyFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(BitmapReader::read(std::istringstream()), Bitmap::BadFile);
}
