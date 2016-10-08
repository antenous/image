/*
 * DirectDrawSurface_test.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include "MockBitmap.hpp"

using namespace image;
using namespace testing;

namespace
{

    class DirectDrawSurfaceTest : public Test
    {
    protected:
        void makeBadFile( std::stringstream & file ) const
        {
            file.setstate( std::ios::badbit );
        }

        void writeFileHeaderWithBadMagic()
        {
            writeToFile< uint32_t >( 0x20534443 );
        }

        void writeFileHeader()
        {
            const uint32_t magic( 0x20534444 );
            const uint32_t size( 124 );
            const uint32_t flags( 0x1 | 0x2 | 0x4 | 0x1000 );
            const uint32_t height( 4 );
            const uint32_t width( 4 );
            const uint32_t pitch( 8 );
            const uint32_t depth( 0 );
            const uint32_t mipmaps( 0 );
            const uint32_t reserved1[11]{ 0 };
            const uint32_t caps( 0x1000 );
            const uint32_t caps2( 0 );
            const uint32_t caps3( 0 );
            const uint32_t caps4( 0 );
            const uint32_t reserved2( 0 );

            writeToFile( magic );
            writeToFile( size );
            writeToFile( flags );
            writeToFile( height );
            writeToFile( width );
            writeToFile( pitch );
            writeToFile( depth );
            writeToFile( mipmaps );
            writeToFile( reserved1 );

            writePixelFormatHeader();

            writeToFile( caps );
            writeToFile( caps2 );
            writeToFile( caps3 );
            writeToFile( caps4 );
            writeToFile( reserved2 );
        }

        void writePixelFormatHeader()
        {
            const uint32_t size( 32 );
            const uint32_t flags( 0x4 );
            const char fourCC[]{ 'D', 'X', 'T', '1' };
            const uint32_t bits( 0 );
            const uint32_t bitMasks[4]{ 0 };

            writeToFile( size );
            writeToFile( flags );
            writeToFile( fourCC );
            writeToFile( bits );
            writeToFile( bitMasks );
        }

        void writeSurfaceData()
        {
            writeToFile( createBlueAndWhiteReferenceColors() );
            writeToFile( createLookUpTableWithBlueTopLeftCorner() );
        }

        uint32_t createBlueAndWhiteReferenceColors() const
        {
            return blue << 16 | white;
        }

        uint32_t createLookUpTableWithBlueTopLeftCorner() const
        {
            return (
                0b00000000 << 24 |
                0b00000000 << 16 |
                0b00000101 << 8  |
                0b00000101 );
        }

        template< typename T >
        void writeToFile( const T & t )
        {
            fileIn.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        void loadDirectDrawSurfaceFromFile()
        {
            writeFileHeader();
            writeSurfaceData();
            dds.loadFrom( fileIn );
        }

        bool hasUnreadData( std::stringstream & file ) const
        {
            return file.peek() != std::istream::traits_type::eof();
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

        Bitmap::Palette createPaletteWithBlueTopLeftCorner() const
        {
            const uint32_t white( 0xffffff );
            const uint32_t blue(  0xff0000 );
            Bitmap::Palette palette( 16, white );
            palette[8] = blue;
            palette[9] = blue;
            palette[12] = blue;
            palette[13] = blue;

            return std::move( palette );
        }

        DirectDrawSurface dds;
        MockBitmap bmp;
        std::stringstream fileIn;
        std::stringstream fileOut;
        const uint16_t blue{ 0x1f };
        const uint16_t white{ 0xffff };
    };
}

TEST_F( DirectDrawSurfaceTest, CanThrowAndCatchBadFile )
{
    try
    {
        throw DirectDrawSurface::BadFile();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad file", e.what() );
    }
}

TEST_F( DirectDrawSurfaceTest, CanThrowAndCatchInvalidType )
{
    try
    {
        throw DirectDrawSurface::InvalidType();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "invalid type", e.what() );
    }
}

TEST_F( DirectDrawSurfaceTest, CanThrowAndCatchBadBitmap )
{
    try
    {
        throw DirectDrawSurface::BadBitmap();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad bitmap", e.what() );
    }
}

TEST_F( DirectDrawSurfaceTest, HasVirtualDestructor )
{
    EXPECT_TRUE( std::has_virtual_destructor< DirectDrawSurface >::value );
}

TEST_F( DirectDrawSurfaceTest, GivenBadFile_WhenLoaded_ThrowsBadFile )
{
    makeBadFile( fileIn );

    EXPECT_THROW( dds.loadFrom( fileIn ), DirectDrawSurface::BadFile );
}

TEST_F( DirectDrawSurfaceTest, GivenEmptyFile_WhenLoaded_ThrowsBadFile )
{
    EXPECT_THROW( dds.loadFrom( fileIn ), DirectDrawSurface::BadFile );
}

TEST_F( DirectDrawSurfaceTest, GivenFileWithInvalidMagic_WhenLoaded_ThrowsInvalidType )
{
    writeFileHeaderWithBadMagic();

    EXPECT_THROW( dds.loadFrom( fileIn ), DirectDrawSurface::InvalidType );
}
TEST_F( DirectDrawSurfaceTest, GivenValidFile_WhenLoaded_ReadsFile )
{
    loadDirectDrawSurfaceFromFile();

    EXPECT_FALSE( hasUnreadData( fileIn ));
}

TEST_F( DirectDrawSurfaceTest, WhenNotLoaded_EvaluatesToFalse )
{
    EXPECT_FALSE( dds );
}

TEST_F( DirectDrawSurfaceTest, WhenFileFailedToLoad_EvaluatesToFalse )
{
    writeFileHeader();

    EXPECT_THROW( dds.loadFrom( fileIn ), DirectDrawSurface::BadFile );
    EXPECT_FALSE( dds );
}

TEST_F( DirectDrawSurfaceTest, WhenFileIsLoaded_HeightIsGettable )
{
    loadDirectDrawSurfaceFromFile();

    EXPECT_EQ( 4, dds.getHeight() );
}

TEST_F( DirectDrawSurfaceTest, WhenFileIsLoaded_WidthIsGettable )
{
    loadDirectDrawSurfaceFromFile();

    EXPECT_EQ( 4, dds.getWidth() );
}

TEST_F( DirectDrawSurfaceTest, WhenFileIsLoaded_SurfaceIsGettable )
{
    loadDirectDrawSurfaceFromFile();

    EXPECT_EQ( 2, dds.getSurface().size() );
}

TEST_F( DirectDrawSurfaceTest, GivenBadFile_WhenSaved_ThrowsBadFile )
{
    makeBadFile( fileOut );

    EXPECT_THROW( dds.saveTo( fileOut ), DirectDrawSurface::BadFile );
}

TEST_F( DirectDrawSurfaceTest, GivenDirectDrawSurfaceIsNotLoaded_WhenSaved_ThrowsInvalidType )
{
    EXPECT_THROW( dds.saveTo( fileOut ), DirectDrawSurface::InvalidType );
}

TEST_F( DirectDrawSurfaceTest, GivenDirectDrawSurfaceIsLoaded_WhenSaved_WritesFile )
{
    loadDirectDrawSurfaceFromFile();
    rewindFile();

    dds.saveTo( fileOut );

    ASSERT_TRUE( hasUnreadData( fileOut ));
    EXPECT_TRUE( filesAreEqual() );
}

TEST_F( DirectDrawSurfaceTest, GivenBitmapNotLoaded_WhenConverted_ThrowsBadBitmap )
{
    EXPECT_THROW( dds.convertFrom( Bitmap() ), DirectDrawSurface::BadBitmap );
}

TEST_F( DirectDrawSurfaceTest, IsConvertibleFromBitmap )
{
    EXPECT_CALL( bmp, getHeight() ).WillRepeatedly( Return( 4 ));
    EXPECT_CALL( bmp, getWidth() ).WillRepeatedly( Return( 4 ));
    EXPECT_CALL( bmp, getPalette() ).WillOnce( Return( createPaletteWithBlueTopLeftCorner() ));

    dds.convertFrom( bmp );
    dds.saveTo( fileOut );

    writeFileHeader();
    writeSurfaceData();

    ASSERT_TRUE( hasUnreadData( fileOut ));
    EXPECT_TRUE( filesAreEqual() );
}
