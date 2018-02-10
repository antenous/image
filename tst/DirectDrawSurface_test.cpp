/*
 * DirectDrawSurface_test.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include "DirectDrawSurfaceReader.hpp"

using namespace image;
using namespace testing;

namespace
{

    class DirectDrawSurfaceTest : public Test
    {
    protected:
        std::ostringstream makeBadFile() const
        {
            std::ostringstream file;
            file.setstate(std::ios::badbit);
            return file;
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
            writeToFile( white );
            writeToFile( blue );
            writeToFile( createLookUpTableWithBlueTopLeftCorner() );
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

        DirectDrawSurface loadDirectDrawSurfaceFromFile()
        {
            writeFileHeader();
            writeSurfaceData();
            return DirectDrawSurfaceReader::read(std::move(fileIn));
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

TEST_F(DirectDrawSurfaceTest, FalseWhenEmpty)
{
    EXPECT_FALSE(DirectDrawSurface());
}

TEST_F(DirectDrawSurfaceTest, Height)
{
    DirectDrawSurface dds{ 0 };
    const auto height(dds.header.height = 4);

    EXPECT_EQ(height, dds.getHeight());
}

TEST_F(DirectDrawSurfaceTest, Width)
{
    DirectDrawSurface dds{ 0 };
    const auto width(dds.header.width = 4);

    EXPECT_EQ(width, dds.getWidth());
}

TEST_F( DirectDrawSurfaceTest, WhenFileIsLoaded_SurfaceIsGettable )
{
    auto dds(loadDirectDrawSurfaceFromFile());

    EXPECT_EQ( 2, dds.getSurface().size() );
}

TEST_F( DirectDrawSurfaceTest, GivenBadFile_WhenSaved_ThrowsBadFile )
{
    auto file(makeBadFile());

    EXPECT_THROW( DirectDrawSurface().save( file ), DirectDrawSurface::BadFile );
}

TEST_F( DirectDrawSurfaceTest, GivenDirectDrawSurfaceIsNotLoaded_WhenSaved_ThrowsInvalidType )
{
    EXPECT_THROW( DirectDrawSurface().save( fileOut ), DirectDrawSurface::InvalidType );
}

TEST_F( DirectDrawSurfaceTest, GivenDirectDrawSurfaceIsLoaded_WhenSaved_WritesFile )
{
    auto dds(loadDirectDrawSurfaceFromFile());
    rewindFile();

    dds.save( fileOut );

    ASSERT_TRUE( hasUnreadData( fileOut ));
    EXPECT_TRUE( filesAreEqual() );
}
