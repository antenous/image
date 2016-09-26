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

        void writeInfoHeader()
        {
            writeToFile( infoHeader.size );
            writeToFile( infoHeader.width );
            writeToFile( infoHeader.height );
            writeToFile( infoHeader.planes );
            writeToFile( infoHeader.bits );
            writeToFile( infoHeader.compression );
            writeToFile( infoHeader.imageSize );
            writeToFile( infoHeader.horizontalResolution );
            writeToFile( infoHeader.verticalResolution );
            writeToFile( infoHeader.colors );
            writeToFile( infoHeader.importantColors );
        }

        Bitmap bitmap;
        Bitmap::FileHeader fileHeader{{ 'B', 'M' }, 313, 0, 0, 54 };
        Bitmap::InfoHeader infoHeader{ 40, 4, 4, 1, 24, 0, 16, 2835, 2835, 0, 0 };
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

TEST_F( BitmapTest, CanThrowAndCatchUnknownInfoHeader )
{
    try
    {
        throw Bitmap::UnknownInfoHeader();
    }
    catch( const std::runtime_error & e )
    {
        EXPECT_STREQ( "unknown info header", e.what() );
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

TEST_F( BitmapTest, GivenFileWithUnknownInfoHeader_WhenRead_ThrowUnknownInfoHeader )
{
    infoHeader.size -= 1;
    writeFileHeader();
    writeInfoHeader();

    EXPECT_THROW( bitmap.read( file ), Bitmap::UnknownInfoHeader );
}

TEST_F( BitmapTest, GivenFileWithValidHeader_WhenRead_ReadFileHeader )
{
    writeFileHeader();
    writeInfoHeader();
    bitmap.read( file );

    EXPECT_EQ( std::istream::traits_type::eof(), file.peek() );
}
