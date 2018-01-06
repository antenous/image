/*
 * ImageConverter_test.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "ImageConverter.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include "BitmapReader.hpp"
#include "BitmapWriter.hpp"
#include "ColorDepth.hpp"

using namespace image;
using namespace testing;

namespace
{
    class ImageConverterTest : public Test
    {
    protected:
        Bitmap makeBitmap() const
        {
            return BitmapReader::read(makeBitmapFile());
        }

        std::stringstream makeBitmapFile() const
        {
            std::stringstream file;
            addBitmapFileHeader( file );
            addBitmapInfoHeader( file );
            addBitmapColorData( file );
            return file;
        }

        void addBitmapFileHeader( std::ostream & out ) const
        {
            const char type[]{ 'B', 'M' };
            const uint32_t offset( 14 + 40 );
            const uint32_t size( offset + 4 * 3 * 4 );
            const uint32_t reserved( 0 );

            write( out, type );
            write( out, size );
            write( out, reserved );
            write( out, offset );
        }

        void addBitmapInfoHeader( std::ostream & out ) const
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

            write( out, size );
            write( out, width );
            write( out, height );
            write( out, planes );
            write( out, bits );
            write( out, compression );
            write( out, imageSize );
            write( out, horizontalResolution );
            write( out, verticalResolution );
            write( out, colors );
            write( out, importantColors );
        }

        void addBitmapColorData( std::ostream & out ) const
        {
            for ( auto color : ColorDepth::highToTrue( uncompressed ))
            {
                write( out, static_cast< uint8_t >(( color >> 16 ) & 0xff ));
                write( out, static_cast< uint8_t >(( color >>  8 ) & 0xff ));
                write( out, static_cast< uint8_t >( color & 0xff ));
            }
        }

        DirectDrawSurface makeDirectDrawSurface() const
        {
            DirectDrawSurface dds;
            auto file( makeDirectDrawSurfaceFile() );
            dds.load( file );
            return dds;
        }

        std::stringstream makeDirectDrawSurfaceFile() const
        {
            std::stringstream file;
            addDirectDrawSurfaceFileHeader( file );
            write( file, swap( referenceColors ));
            write( file, lookup );
            return file;
        }

        void addDirectDrawSurfaceFileHeader( std::ostream & out ) const
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

            write( out, magic );
            write( out, size );
            write( out, flags );
            write( out, height );
            write( out, width );
            write( out, pitch );
            write( out, depth );
            write( out, mipmaps );
            write( out, reserved1 );

            writePixelFormatHeader( out );

            write( out, caps );
            write( out, caps2 );
            write( out, caps3 );
            write( out, caps4 );
            write( out, reserved2 );
        }

        template< typename T >
        void write( std::ostream & file, const T & t ) const
        {
            file.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        void writePixelFormatHeader( std::ostream & out ) const
        {
            const uint32_t size( 32 );
            const uint32_t flags( 0x4 );
            const char fourCC[]{ 'D', 'X', 'T', '1' };
            const uint32_t bits( 0 );
            const uint32_t bitMasks[4]{ 0 };

            write( out, size );
            write( out, flags );
            write( out, fourCC );
            write( out, bits );
            write( out, bitMasks );
        }

        uint32_t swap( uint32_t colors ) const
        {
            return (( colors & 0x0000ffff ) << 16 )|
                   (( colors & 0xffff0000 ) >> 16 );
        }

        bool filesAreEqual( std::istream & a, std::istream & b ) const
        {
            return std::equal(
                std::istreambuf_iterator< char >( a ), std::istreambuf_iterator< char >(),
                std::istreambuf_iterator< char >( b ));
        }

        const std::vector< uint16_t > original{{
            131, 170, 241, 313,
            282, 281, 162, 161,
            221, 313, 131, 222,
            250, 132, 312, 190 }};

        const uint32_t referenceColors{ 313 << 16 | 131 };

        const uint32_t lookup{
            ( 0 << 6 | 2 << 4 | 3 << 2 | 1 ) << 24 |
            ( 3 << 6 | 3 << 4 | 0 << 2 | 0 ) << 16 |
            ( 2 << 6 | 1 << 4 | 0 << 2 | 2 ) <<  8 |
            ( 3 << 6 | 0 << 4 | 1 << 2 | 2 )};

        std::vector< uint32_t > compressed{ referenceColors, lookup };

        const std::vector< uint16_t > uncompressed{{
            131, 170, 241, 313,
            313, 313, 170, 170,
            241, 313, 131, 241,
            241, 131, 313, 170 }};
    };
}

TEST_F( ImageConverterTest, CanThrowAndCatchBadBitmap )
{
    try
    {
        throw ImageConverter::BadBitmap();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad bitmap", e.what() );
    }
}

TEST_F( ImageConverterTest, GivenBadBitmap_WhenConverted_ThrowsBadBitmap )
{
    EXPECT_THROW( ImageConverter::convert( Bitmap() ), ImageConverter::BadBitmap );
}

TEST_F( ImageConverterTest, ConvertBitmap )
{
    auto expected( makeDirectDrawSurfaceFile() );

    std::stringstream result;
    ImageConverter::convert( makeBitmap() ).save( result );

    EXPECT_TRUE( filesAreEqual( expected, result ));
}

TEST_F( ImageConverterTest, CanThrowAndCatchBadDirectDrawSurface )
{
    try
    {
        throw ImageConverter::BadDirectDrawSurface();
    }
    catch ( const std::runtime_error & e )
    {
        EXPECT_STREQ( "bad direct draw surface", e.what() );
    }
}

TEST_F( ImageConverterTest, GivenBadDirectDrawSurface_WhenConverted_ThrowsBadDirectDrawSurface )
{
    EXPECT_THROW( ImageConverter::convert( DirectDrawSurface() ), ImageConverter::BadDirectDrawSurface );
}

TEST_F( ImageConverterTest, ConvertDirectDrawSurface )
{
    auto expected( makeBitmapFile() );

    std::stringstream result;
    BitmapWriter::write( std::move(result), ImageConverter::convert( makeDirectDrawSurface() ));

    EXPECT_TRUE( filesAreEqual( expected, result ));
}
