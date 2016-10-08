/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <cstring>
#include <istream>
#include "BitmapConverter.hpp"
#include "DirectDrawSurface.hpp"

using namespace image;

namespace
{
    template< typename T >
    void read( std::istream & file, T & t )
    {
        file.read( reinterpret_cast< char* >( &t ), sizeof( t ));
    }

    template< typename T >
    void write( std::ostream & file, const T & t )
    {
        file.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
    }

    bool isBitmap( const char type[2] )
    {
        return type[0] == 'B' && type[1] == 'M';
    }

    uint32_t readColor( std::istream & file )
    {
        uint8_t blue, green, red;

        read( file, blue );
        read( file, green );
        read( file, red );

        return blue << 16 | green << 8 | red;
    }

    void writeColor( std::ostream & file, uint32_t color )
    {
        const uint8_t blue(( color >> 16 ) & 0xff );
        const uint8_t green(( color >> 8 ) & 0xff );
        const uint8_t red( color & 0xff );

        write( file, blue );
        write( file, green );
        write( file, red );
    }

    void skipPadding( std::istream & file, std::istream::streamoff off )
    {
        file.seekg( off, file.cur );
    }

    void addPadding( std::ostream & file, uint8_t bytes )
    {
        char padding[] = { 0, 0, 0 };
        file.write( padding, bytes );
    }

    uint8_t countPadding( uint16_t bits, int32_t width )
    {
        const auto bytesInRow(( bits * width + 31 ) / 32 * 4 );
        return bytesInRow - width * 3;
    }
}

Bitmap::operator bool() const
{
    return isBitmap( fileHeader.type );
}

void Bitmap::loadFrom( std::istream & file )
{
    if ( !file )
        throw BadFile();

    file.exceptions( std::istream::failbit | std::istream::badbit );

    try
    {
        readFileHeader( file );
        readInfoHeader( file );
        readPalette( file );
    }
    catch ( const std::istream::failure & )
    {
        fileHeader.type[0] = 0;
        throw BadFile();
    }
}

void Bitmap::convertFrom( const DirectDrawSurface & dds )
{
    if ( !dds )
        throw BadDirectDrawSurface();

    palette = BitmapConverter().convert( dds );

    createInfoHeader( dds );
    createFileHeader();
}

void Bitmap::createInfoHeader( const DirectDrawSurface & dds )
{
    memset( &infoHeader, 0, sizeof( infoHeader ));

    infoHeader.size = 40;
    infoHeader.width = dds.getWidth();
    infoHeader.height = dds.getHeight();
    infoHeader.planes = 1;
    infoHeader.bits = 24;

    padding = countPadding( infoHeader.bits, infoHeader.width );

    infoHeader.imageSize = palette.size() * 3 + padding * infoHeader.width;
}

void Bitmap::createFileHeader()
{
    memset( &fileHeader, 0 , sizeof( fileHeader ));

    fileHeader.type[0] = 'B';
    fileHeader.type[1] = 'M';
    fileHeader.size = 14;
    fileHeader.offset = 54;
    fileHeader.size += infoHeader.size + infoHeader.imageSize;
}

void Bitmap::saveTo( std::ostream & file ) const
{
    if ( !file )
        throw BadFile();

    writeFileHeader( file );
    writeInfoHeader( file );
    writePalette( file );
}

int32_t Bitmap::getHeight() const
{
    return infoHeader.height;
}

int32_t Bitmap::getWidth() const
{
    return infoHeader.width;
}

Bitmap::Palette Bitmap::getPalette() const
{
    return palette;
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

    padding = countPadding( infoHeader.bits, infoHeader.width );
}

void Bitmap::readPalette( std::istream & file )
{
    palette.clear();

    for ( int32_t row( 0 ); row < infoHeader.height; ++row, skipPadding( file, padding ))
        for ( int32_t column( 0 ); column < infoHeader.width; ++column )
            palette.push_back( readColor( file ));
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

void Bitmap::writePalette( std::ostream & file ) const
{
    for ( Palette::size_type row( 0 ), end( infoHeader.height ); row < end; ++row )
        writePaletteRow( file, row );
}

void Bitmap::writePaletteRow( std::ostream & file, typename Palette::size_type row ) const
{
    for ( auto i( row * infoHeader.width ), end( i + infoHeader.width ); i < end; ++i )
        writeColor( file, palette[i] );

    addPadding( file, padding );
}
