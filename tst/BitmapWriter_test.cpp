/*
 * BitmapWriter_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapWriter.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BitmapWriterTest : public Test
    {
    protected:
        std::ostringstream makeBadFile() const
        {
            std::ostringstream file;
            file.setstate(std::ios::badbit);
            return file;
        }
    };
}

TEST_F(BitmapWriterTest, GivenBadFile_WhenWritten_ThrowsBadFile)
{
    EXPECT_THROW(BitmapWriter::write(makeBadFile(), Bitmap()), Bitmap::BadFile);
}
