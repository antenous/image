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
        void createFileHeaderWithInvalidType()
        {
            char type[]{ 'M', 'B' };
            writeToFile( type );
        }

        void createFileHeader()
        {
            char type[]{ 'B', 'M' };
            uint32_t size( 102 );
            uint32_t reserved( 0 );
            uint32_t offset( 54 );

            writeToFile( type );
            writeToFile( size );
            writeToFile( reserved );
            writeToFile( offset );
        }

        void createInfoHeader()
        {
            uint32_t size( 40 );
            int32_t width( 4 );
            int32_t height( 4 );
            uint16_t planes( 1 );
            uint16_t bits( 24 );
            uint32_t compression( 0 );
            uint32_t imageSize( 16 );
            uint32_t horizontalResolution( 2835 );
            uint32_t verticalResolution( 2835 );
            uint32_t colors( 0 );
            uint32_t importantColors( 0 );

            writeToFile( size );
            writeToFile( width );
            writeToFile( height );
            writeToFile( planes );
            writeToFile( bits );
            writeToFile( compression );
            writeToFile( imageSize );
            writeToFile( horizontalResolution );
            writeToFile( verticalResolution );
            writeToFile( colors );
            writeToFile( importantColors );
        }

        template< typename T >
        void writeToFile( const T & t )
        {
            file.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        Bitmap bitmap;
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

TEST_F( BitmapTest, GivenBadFile_WhenLoaded_ThrowsBadFile )
{
    file.setstate( std::ios::badbit );
    EXPECT_THROW( bitmap.loadFrom( file ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenEmptyFile_WhenLoaded_ThrowsBadFile )
{
    EXPECT_THROW( bitmap.loadFrom( file ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenFileWithInvalidType_WhenLoaded_ThrowsInvalidType )
{
    createFileHeaderWithInvalidType();

    EXPECT_THROW( bitmap.loadFrom( file ), Bitmap::InvalidType );
}

TEST_F( BitmapTest, GivenFileWithValidHeaders_WhenLoaded_ReadsFileHeaders )
{
    createFileHeader();
    createInfoHeader();
    bitmap.loadFrom( file );

    EXPECT_EQ( std::istream::traits_type::eof(), file.peek() );
}
