/*
 * DirectDrawSurfaceWriter_test.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceWriter.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class DirectDrawSurfaceWriterTest : public Test
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

TEST_F(DirectDrawSurfaceWriterTest, GivenBadFile_WhenWritten_ThrowsBadFile)
{
    EXPECT_THROW(DirectDrawSurfaceWriter::write(makeBadFile(), DirectDrawSurface()), DirectDrawSurface::BadFile);
}
