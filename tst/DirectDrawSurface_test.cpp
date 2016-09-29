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

        template< typename T >
        void writeToFile( const T & t )
        {
            file.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        bool hasUnreadData( std::stringstream & file ) const
        {
            return file.peek() != std::istream::traits_type::eof();
        }

        DirectDrawSurface dds;
        std::stringstream file;
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
    makeBadFile( file );

    EXPECT_THROW( dds.loadFrom( file ), DirectDrawSurface::BadFile );
}

TEST_F( DirectDrawSurfaceTest, GivenEmptyFile_WhenLoaded_ThrowsBadFile )
{
    EXPECT_THROW( dds.loadFrom( file ), DirectDrawSurface::BadFile );
}

TEST_F( DirectDrawSurfaceTest, GivenFileWithInvalidMagic_WhenLoaded_ThrowsInvalidType )
{
    createFileHeaderWithBadMagic();

    EXPECT_THROW( dds.loadFrom( file ), DirectDrawSurface::InvalidType );
}

TEST_F( DirectDrawSurfaceTest, GivenValidHeader_WhenLoaded_ReadsHeader )
{
    createFileHeader();
    dds.loadFrom( file );

    EXPECT_FALSE( hasUnreadData( file ));
}
