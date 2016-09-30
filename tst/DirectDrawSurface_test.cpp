/*
 * DirectDrawSurface_test.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include <algorithm>
#include <gtest/gtest.h>

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

        void createFileHeaderWithBadMagic()
        {
            uint32_t badMagic( 0x20534443 );
            writeToFile( badMagic );
        }

        void createFileHeader()
        {
            uint32_t magic( 0x20534444 );
            uint32_t size( 124 );
            uint32_t flags( 0x1 | 0x2 | 0x4 | 0x1000 );
            uint32_t height( 4 );
            uint32_t width( 4 );
            uint32_t pitch( std::max( 1U, (( width + 3 ) / 4 )) * 8 );
            uint32_t depth( 0 );
            uint32_t mipmaps( 0 );
            uint32_t reserved1[11] = { 0 };
            uint32_t caps( 0x1000 );
            uint32_t caps2( 0 );
            uint32_t caps3( 0 );
            uint32_t caps4( 0 );
            uint32_t reserved2( 0 );

            writeToFile( magic );
            writeToFile( size );
            writeToFile( flags );
            writeToFile( height );
            writeToFile( width );
            writeToFile( pitch );
            writeToFile( depth );
            writeToFile( mipmaps );
            writeToFile( reserved1 );

            createPixelFormatHeader();

            writeToFile( caps );
            writeToFile( caps2 );
            writeToFile( caps3 );
            writeToFile( caps4 );
            writeToFile( reserved2 );
        }

        void createPixelFormatHeader()
        {
            uint32_t size( 32 );
            uint32_t flags( 0x4 );
            char fourCC[] = { 'D', 'X', 'T', '1' };
            uint32_t bits( 0 );
            uint32_t bitMasks[4]{ 0 };

            writeToFile( size );
            writeToFile( flags );
            writeToFile( fourCC );
            writeToFile( bits );
            writeToFile( bitMasks );
        }

        void createSurfaceData()
        {
            createBlueReferenceColor();
            createWhiteReferenceColor();

            createLookUpTableWithBlueTopLeftCorner();
        }

        void createWhiteReferenceColor()
        {
            createColor( 255, 255, 255 );
        }

        void createBlueReferenceColor()
        {
            createColor( 0, 0, 255 );
        }

        void createColor( uint8_t red, uint8_t green, uint8_t blue )
        {
            const uint16_t color( createRedComponent( red ) | createGreenComponent( green ) | createBlueComponent( blue ));
            writeToFile( color );
        }

        uint16_t createRedComponent( uint8_t red ) const
        {
            const uint16_t color( dropLeastSignificantBits( red, redBits ));
            return color << ( greenBits + blueBits );
        }

        uint16_t createGreenComponent( uint8_t green ) const
        {
            const uint16_t color( dropLeastSignificantBits( green, greenBits ));
            return color << blueBits;
        }

        uint16_t createBlueComponent( uint8_t blue ) const
        {
            return dropLeastSignificantBits( blue, blueBits );
        }

        uint8_t dropLeastSignificantBits( uint8_t color, uint8_t significantBitsToKeep ) const
        {
            return color >> ( 8 - significantBitsToKeep );
        }

        void createLookUpTableWithBlueTopLeftCorner()
        {
            uint32_t table( 0 );
            table |= 0b01010101 << 24;
            table |= 0b01010101 << 16;
            table |= 0b01010000 << 8;
            table |= 0b01010000;

            writeToFile( table );
        }

        template< typename T >
        void writeToFile( const T & t )
        {
            fileIn.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        void loadDirectDrawSurfaceFromFile()
        {
            createFileHeader();
            createSurfaceData();
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

        DirectDrawSurface dds;
        std::stringstream fileIn;
        std::stringstream fileOut;
        const uint8_t blueBits{ 5 };
        const uint8_t greenBits{ 6 };
        const uint8_t redBits{ 5 };
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
    createFileHeaderWithBadMagic();

    EXPECT_THROW( dds.loadFrom( fileIn ), DirectDrawSurface::InvalidType );
}
TEST_F( DirectDrawSurfaceTest, GivenValidFile_WhenLoaded_ReadsFile )
{
    loadDirectDrawSurfaceFromFile();

    EXPECT_FALSE( hasUnreadData( fileIn ));
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
