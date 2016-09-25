/*
 * Bitmap_test.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BitmapTest : public Test
    {
    protected:
        template< typename T >
        void writeToFile( const T & t )
        {
            file.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        void writeFileHeader()
        {
            writeToFile( fileHeader.type );
            writeToFile( fileHeader.size );
            writeToFile( fileHeader.reserved1 );
            writeToFile( fileHeader.reserved2 );
            writeToFile( fileHeader.offset );
        }

        Bitmap bitmap;
        Bitmap::FileHeader fileHeader{{ 'B', 'M' }, 313, 0, 0, 54 };
        std::stringstream file;
    };
}

TEST_F( BitmapTest, CanThrowAndCatchBadFile )
{
    try
    {
        throw Bitmap::BadFile();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad file", e.what() );
    }
}

TEST_F( BitmapTest, CanThrowAndCatchInvalidType )
{
    try
    {
        throw Bitmap::InvalidType();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "invalid type", e.what() );
    }
}

TEST_F( BitmapTest, GivenBadFile_WhenRead_ThrowBadFile )
{
    file.setstate( std::ios::badbit );
    ASSERT_TRUE( file.bad() );
    EXPECT_THROW( bitmap.read( file ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenEmptyFile_WhenRead_ThrowBadFile )
{
    EXPECT_THROW( bitmap.read( file ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenFileShorterThanFileHeader_WhenRead_ThrowBadFile )
{
    writeFileHeader();
    char c;
    file.read( &c, sizeof( c ));

    EXPECT_THROW( bitmap.read( file ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenFileWithInvalidType_WhenRead_ThrowInvalidType )
{
    std::swap( fileHeader.type[0], fileHeader.type[1] );
    writeFileHeader();

    EXPECT_THROW( bitmap.read( file ), Bitmap::InvalidType );
}

TEST_F( BitmapTest, GivenFileWithValidHeader_WhenRead_ReadFileHeader )
{
    writeFileHeader();
    bitmap.read( file );

    EXPECT_EQ( std::istream::traits_type::eof(), file.peek() );
}
