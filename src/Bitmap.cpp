/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <istream>
#include "BitmapDataReader.hpp"

using namespace image;

namespace
{
    bool isBitmap( const char type[2] )
    {
        return type[0] == 'B' && type[1] == 'M';
    }

    template< typename T >
    void read( std::istream & file, T & t )
    {
        file.read( reinterpret_cast< char* >( &t ), sizeof( t ));
    }

    inline uint32_t blue( uint8_t blue )
    {
        return blue << 16;
    }

    inline uint32_t green( uint8_t green )
    {
        return green << 8;
    }

    inline uint32_t red( uint8_t red )
    {
        return red;
    }

    template< typename T >
    void write( std::ostream & file, const T & t )
    {
        file.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
    }

    inline uint8_t blue( uint32_t color )
    {
        return (( color >> 16 ) & 0xff );
    }

    inline uint8_t green( uint32_t color )
    {
        return (( color >> 8 ) & 0xff );
    }

    inline uint8_t red( uint32_t color )
    {
        return ( color & 0xff );
    }

    void addPadding( std::ostream & file, uint8_t bytes )
    {
        const char padding[] = { 0, 0, 0 };
        file.write( padding, bytes );
    }

    void writeData( std::ostream & file, const std::vector< uint8_t > & data, int32_t height, int32_t width, std::istream::streamoff padding )
    {
        for ( int32_t y( 0 ); y < height; ++y, addPadding( file, padding ))
            file.write( reinterpret_cast< const char* >( &data[ y * width * 3 ] ), width * 3 );
    }
}

Bitmap::operator bool() const
{
    return isBitmap( fileHeader.type );
}

void Bitmap::load( std::istream & file )
{
    if ( !file )
        throw BadFile();

    file.exceptions( std::istream::failbit | std::istream::badbit );

    try
    {
        readFileHeader( file );
        readInfoHeader( file );
        readColors( file );
    }
    catch ( const std::istream::failure & )
    {
        fileHeader.type[0] = 0;
        throw BadFile();
    }
}

void Bitmap::readFileHeader( std::istream & file )
{
    read( file, fileHeader.type );

    if ( !isBitmap( fileHeader.type ))
        throw InvalidType();

    read( file, fileHeader.size );
    read( file, fileHeader.reserved1 );
    read( file, fileHeader.reserved2 );
    read( file, fileHeader.offset );
}

void Bitmap::readInfoHeader( std::istream & file )
{
    read( file, infoHeader.size );
    read( file, infoHeader.width );
    read( file, infoHeader.height );
    read( file, infoHeader.planes );
    read( file, infoHeader.bits );
    read( file, infoHeader.compression );
    read( file, infoHeader.imageSize );
    read( file, infoHeader.horizontalResolution );
    read( file, infoHeader.verticalResolution );
    read( file, infoHeader.colors );
    read( file, infoHeader.importantColors );
}

void Bitmap::readColors( std::istream & file )
{
    colors.resize( infoHeader.height * infoHeader.width );
    auto it( colors.begin() );

    for ( auto & data : BitmapDataReader( file, infoHeader.width, infoHeader.height ))
        for ( auto first( data.begin() ), last( data.end() ); first != last; std::advance( first, 3 ), ++it )
            *it = blue( *std::next( first, 0 )) | green( *std::next( first, 1 )) | red( *std::next( first, 2 ));
}

void Bitmap::save( std::ostream & file ) const
{
    if ( !file )
        throw BadFile();

    writeFileHeader( file );
    writeInfoHeader( file );
    writeColors( file );
}

void Bitmap::writeFileHeader( std::ostream & file ) const
{
    if ( !isBitmap( fileHeader.type ))
        throw InvalidType();

    write( file, fileHeader.type );
    write( file, fileHeader.size );
    write( file, fileHeader.reserved1 );
    write( file, fileHeader.reserved2 );
    write( file, fileHeader.offset );
}

void Bitmap::writeInfoHeader( std::ostream & file ) const
{
    write( file, infoHeader.size );
    write( file, infoHeader.width );
    write( file, infoHeader.height );
    write( file, infoHeader.planes );
    write( file, infoHeader.bits );
    write( file, infoHeader.compression );
    write( file, infoHeader.imageSize );
    write( file, infoHeader.horizontalResolution );
    write( file, infoHeader.verticalResolution );
    write( file, infoHeader.colors );
    write( file, infoHeader.importantColors );
}

void Bitmap::writeColors( std::ostream & file ) const
{
    writeData( file, colorsToData(), infoHeader.height, infoHeader.width, padding() );
}

Bitmap::Data Bitmap::colorsToData() const
{
    Data data( colors.size() * 3 );
    auto it( data.begin() );

    for ( auto first( colors.begin() ), last( colors.end() ); first != last; ++first, std::advance( it, 3 ))
    {
        *std::next( it, 0 ) = blue( *first );
        *std::next( it, 1 ) = green( *first );
        *std::next( it, 2 ) = red( *first );
    }

    return data;
}

int32_t Bitmap::getHeight() const
{
    return infoHeader.height;
}

int32_t Bitmap::getWidth() const
{
    return infoHeader.width;
}

Bitmap::Colors Bitmap::getColors() const
{
    return colors;
}

uint8_t Bitmap::padding() const
{
    const auto bytesInRow(( infoHeader.bits * infoHeader.width + 31 ) / 32 * 4 );
    return bytesInRow - infoHeader.width * 3;
}
