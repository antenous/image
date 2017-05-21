/*
 * BitmapDataReader_test.cpp
 *
 *  Created on: May 5, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapDataReader.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BitmapDataReaderTest : public Test
    {
    protected:
        std::stringstream file;
        const std::vector< uint8_t > data{ 3, 1, 4, 1, 5, 9, 2, 6 };
    };
}

TEST_F( BitmapDataReaderTest, GivenNoData_RangeIsEmpty )
{
    for ( auto & line : BitmapDataReader( file, 0, 0 ))
        FAIL() << "Range not empty : " << line.size();
}

TEST_F( BitmapDataReaderTest, BeforeReferenced_DataIsNotRead )
{
    file.write( reinterpret_cast< const char* >( &data[0] ), data.size() );
    BitmapDataReader reader( file, 1, 2 );

    auto it( reader.begin() );
    EXPECT_EQ( 0, file.tellg() );

    ++it;
    EXPECT_EQ( 0, file.tellg() );
}

TEST_F( BitmapDataReaderTest, WhenReferenced_DataIsReadOnce )
{
    file.write( reinterpret_cast< const char* >( &data[0] ), data.size() );
    BitmapDataReader reader( file, 1, 2 );

    auto it( reader.begin() );

    *it;
    EXPECT_EQ( 4, file.tellg() );

    *it;
    EXPECT_EQ( 4, file.tellg() );
}

TEST_F( BitmapDataReaderTest, GivenIncrementedFirst_WhenReferenced_DataIsReadTwice )
{
    file.write( reinterpret_cast< const char* >( &data[0] ), data.size() );
    BitmapDataReader reader( file, 1, 2 );

    auto it( ++reader.begin() );

    *it;
    EXPECT_EQ( 8, file.tellg() );
}

TEST_F( BitmapDataReaderTest, ReadData )
{
    file.write( reinterpret_cast< const char* >( &data[0] ), data.size() );
    unsigned i( 0 );

    for ( auto & line : BitmapDataReader( file, 1, 2 ))
        EXPECT_TRUE( std::equal( line.begin(), line.end(), std::next( data.begin(), i++ * 4 )));

    EXPECT_TRUE( file.good() );
}

TEST_F( BitmapDataReaderTest, CanThrowAndCatchSubsequentCallToBegin )
{
    try
    {
        throw BitmapDataReader::SubsequentCallToBegin();
    }
    catch ( const std::logic_error & e )
    {
        EXPECT_STREQ( "subsequent call to begin", e.what() );
    }
}

TEST_F( BitmapDataReaderTest, GivenBeginIsCalled_SubsequentCallsWillThrow )
{
    BitmapDataReader reader( file, 0, 0 );

    reader.begin();
    EXPECT_THROW( reader.begin(), BitmapDataReader::SubsequentCallToBegin );
}
