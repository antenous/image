/*
 * Bitmap_test.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <gtest/gtest.h>
#include "MockDirectDrawSurface.hpp"

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

        void writeFileHeader()
        {
            const char type[]{ 'B', 'M' };
            const uint32_t size( 102 );
            const uint32_t reserved( 0 );
            const uint32_t offset( 54 );

            writeToFile( type );
            writeToFile( size );
            writeToFile( reserved );
            writeToFile( offset );
        }

        void writeConvertedFileHeader()
        {
            const char type[]{ 'B', 'M' };
            const uint32_t offset( 14 + 40 );
            const uint32_t size( offset + 4 * 3 * 4 );
            const uint32_t reserved( 0 );

            writeToFile( type );
            writeToFile( size );
            writeToFile( reserved );
            writeToFile( offset );
        }

        void writeInfoHeader()
        {
            const uint32_t size( 40 );
            const int32_t width( 2 );
            const int32_t height( 2 );
            const uint16_t planes( 1 );
            const uint16_t bits( 24 );
            const uint32_t compression( 0 );
            const uint32_t imageSize( 16 );
            const uint32_t horizontalResolution( 0 );
            const uint32_t verticalResolution( 0 );
            const uint32_t colors( 0 );
            const uint32_t importantColors( 0 );

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

        void writeConvertedInfoHeader()
        {
            const uint32_t size( 40 );
            const int32_t width( 4 );
            const int32_t height( 4 );
            const uint16_t planes( 1 );
            const uint16_t bits( 24 );
            const uint32_t compression( 0 );
            const uint32_t imageSize( 4 * 3 * 4 );
            const uint32_t horizontalResolution( 0 );
            const uint32_t verticalResolution( 0 );
            const uint32_t colors( 0 );
            const uint32_t importantColors( 0 );

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

        void writeColorTable()
        {
            createRedPixel();
            createWhitePixel();
            createPadding();
            createBluePixel();
            createGreenPixel();
            createPadding();
        }

        void writeConvertedColorTable()
        {
            createWhitePixel();
            createWhitePixel();
            createWhitePixel();
            createWhitePixel();

            createWhitePixel();
            createWhitePixel();
            createWhitePixel();
            createWhitePixel();

            createBluePixel();
            createBluePixel();
            createWhitePixel();
            createWhitePixel();

            createBluePixel();
            createBluePixel();
            createWhitePixel();
            createWhitePixel();
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
            writeFileHeader();
            writeInfoHeader();
            writeColorTable();
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

        uint32_t createLookupTableWithBlueTopLeftCorner() const
        {
            return (
                0b00000000 << 24 |
                0b00000000 << 16 |
                0b00000101 << 8  |
                0b00000101 );
        }

        DirectDrawSurface::Surface createSurface()
        {
            const uint16_t blue{ 0x1f };
            const uint16_t white{ 0xffff };
            const uint32_t reference( white << 16 | blue );
            const auto lookup( createLookupTableWithBlueTopLeftCorner() );
            return { reference, lookup };
        };

        Bitmap bitmap;
        MockDirectDrawSurface dds;
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

TEST_F( BitmapTest, CanThrowAndCatchBadDirectDrawSurface )
{
    try
    {
        throw Bitmap::BadDirectDrawSurface();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad direct draw surface", e.what() );
    }
}

TEST_F( BitmapTest, HasVirtualDestructor )
{
    EXPECT_TRUE( std::has_virtual_destructor< Bitmap >::value );
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

TEST_F( BitmapTest, WhenNotLoaded_EvaluatesToFalse )
{
    EXPECT_FALSE( bitmap );
}

TEST_F( BitmapTest, WhenFileFailedToLoad_EvaluatesToFalse )
{
    writeFileHeader();

    EXPECT_THROW( bitmap.loadFrom( fileIn ), Bitmap::BadFile );
    EXPECT_FALSE( bitmap );
}

TEST_F( BitmapTest, GivenValidFile_WhenLoaded_ReadsFile )
{
    loadBitmapFromFile();

    EXPECT_FALSE( hasUnreadData( fileIn ));
}

TEST_F( BitmapTest, WhenBitmapIsLoaded_HeightIsGettable )
{
    loadBitmapFromFile();

    EXPECT_EQ( 2, bitmap.getHeight() );
}

TEST_F( BitmapTest, WhenBitmapIsLoaded_WidthIsGettable )
{
    loadBitmapFromFile();

    EXPECT_EQ( 2, bitmap.getWidth() );
}

TEST_F( BitmapTest, WhenBitmapIsLoaded_PaletteIsGettable )
{
    loadBitmapFromFile();

    EXPECT_EQ( 4, bitmap.getPalette().size() );
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

TEST_F( BitmapTest, GivenDirectDrawSurfaceNotLoaded_WhenConverted_ThrowsBadDirectDrawSurface )
{
    EXPECT_THROW( bitmap.convertFrom( DirectDrawSurface() ), Bitmap::BadDirectDrawSurface );
}

TEST_F( BitmapTest, IsConvertibleFromDirectDrawSurface )
{
    const auto surface( createSurface() );

    EXPECT_CALL( dds, getSurface() ).WillOnce( Return( surface ));
    EXPECT_CALL( dds, getHeight() ).WillRepeatedly( Return( 4 ));
    EXPECT_CALL( dds, getWidth() ).WillRepeatedly( Return( 4 ));

    bitmap.convertFrom( dds );
    bitmap.saveTo( fileOut );

    writeConvertedFileHeader();
    writeConvertedInfoHeader();
    writeConvertedColorTable();

    ASSERT_TRUE( hasUnreadData( fileOut ));
    EXPECT_TRUE( filesAreEqual() );
}
