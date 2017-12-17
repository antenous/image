/*
 * DirectDrawSurfaceReader_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceReader.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class DirectDrawSurfaceReaderTest : public Test
    {

    };
}

TEST_F(DirectDrawSurfaceReaderTest, GivenEmptyFile_WhenRead_ThrowsBadFile)
{
    EXPECT_THROW(DirectDrawSurfaceReader::read(std::istringstream()), DirectDrawSurface::BadFile);
}
