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

        void makeBadFile( std::stringstream & file ) const
        {
            file.setstate( std::ios::badbit );
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
            int32_t width( 2 );
            int32_t height( 2 );
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

        void createColorTable()
        {
            createRedPixel();
            createWhitePixel();
            createPadding();
            createBluePixel();
            createGreenPixel();
            createPadding();
        }

        void createRedPixel()
        {
            writeToFile({ 0, 0, 255 });
        }

        void createWhitePixel()
        {
            writeToFile({ 255, 255, 255 });
        }

        void createPadding()
        {
            writeToFile({ 0, 0 });
        }

        void createBluePixel()
        {
            writeToFile({ 255, 0 , 0 });
        }

        void createGreenPixel()
        {
            writeToFile({ 0, 255, 0 });
        }

        template< typename T >
        void writeToFile( const T & t )
        {
            fileIn.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        void writeToFile( const std::vector< uint8_t > & bytes )
        {
            for ( auto byte : bytes )
                writeToFile( byte );
        }

        bool hasUnreadData( std::stringstream & file ) const
        {
            return file.peek() != std::istream::traits_type::eof();
        }

        void loadBitmapFromFile()
        {
            createFileHeader();
            createInfoHeader();
            createColorTable();
            bitmap.loadFrom( fileIn );
        }

        void rewindFile()
        {
            fileIn.seekg( 0, fileIn.beg );
        }

        bool filesAreEqual()
        {
            return std::equal(
                std::istreambuf_iterator< char >( fileOut ), std::istreambuf_iterator< char >(),
                std::istreambuf_iterator< char >( fileIn ));
        }

        Bitmap bitmap;
        std::stringstream fileIn;
        std::stringstream fileOut;
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
    makeBadFile( fileIn );

    EXPECT_THROW( bitmap.loadFrom( fileIn ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenEmptyFile_WhenLoaded_ThrowsBadFile )
{
    EXPECT_THROW( bitmap.loadFrom( fileIn ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenFileWithInvalidType_WhenLoaded_ThrowsInvalidType )
{
    createFileHeaderWithInvalidType();

    EXPECT_THROW( bitmap.loadFrom( fileIn ), Bitmap::InvalidType );
}

TEST_F( BitmapTest, GivenValidFile_WhenLoaded_ReadsFile )
{
    loadBitmapFromFile();

    EXPECT_FALSE( hasUnreadData( fileIn ));
}

TEST_F( BitmapTest, GivenBadFile_WhenSaved_ThrowsBadFile )
{
    makeBadFile( fileOut );

    EXPECT_THROW( bitmap.saveTo( fileOut ), Bitmap::BadFile );
}

TEST_F( BitmapTest, GivenBitmapIsNotLoaded_WhenSaved_ThrowsInvalidType )
{
    EXPECT_THROW( bitmap.saveTo( fileOut ), Bitmap::InvalidType );
}

TEST_F( BitmapTest, GivenBitmapIsLoaded_WhenSaved_WritesFile )
{
    loadBitmapFromFile();
    rewindFile();

    bitmap.saveTo( fileOut );

    ASSERT_TRUE( hasUnreadData( fileOut ));
    EXPECT_TRUE( filesAreEqual() );
}
