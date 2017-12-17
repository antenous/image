/*
 * DirectDrawSurface.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include <istream>

using namespace image;

namespace
{
    bool isDirectDrawSurface( uint32_t magic )
    {
        return magic == 0x20534444;
    }

    uint32_t swap( uint32_t colors )
    {
        return (( colors & 0x0000ffff ) << 16 )|
               (( colors & 0xffff0000 ) >> 16 );
    }

    template< typename T >
    void read( std::istream & file, T & t )
    {
        file.read( reinterpret_cast< char* >( &t ), sizeof( t ));
    }

    void readReferenceColors( std::istream & file, uint32_t & colors )
    {
        read( file, colors );
        colors = swap( colors );
    }

    template< typename T >
    void write( std::ostream & file, const T & t )
    {
        file.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
    }

    void writeReferenceColors( std::ostream & file, uint32_t colors )
    {
        colors = swap( colors );
        write( file, colors );
    }
}

DirectDrawSurface::operator bool() const
{
    return isDirectDrawSurface( magic );
}

void DirectDrawSurface::load( std::istream & file )
{
    if ( !file )
        throw BadFile();

    file.exceptions( std::istream::failbit | std::istream::badbit );

    try
    {
        readMagic( file );
        readHeader( file );
        readSurfaceData( file );
    }
    catch ( const std::istream::failure & )
    {
        magic = 0;
        throw BadFile();
    }
}

void DirectDrawSurface::save( std::ostream & file ) const
{
    if ( !file )
        throw BadFile();

    writeMagic( file );
    writeHeader( file );
    writeSurfaceData( file );
}

uint32_t DirectDrawSurface::getHeight() const
{
    return header.height;
}

uint32_t DirectDrawSurface::getWidth() const
{
    return header.width;
}

DirectDrawSurface::Surface DirectDrawSurface::getSurface() const
{
    return surface;
}

void DirectDrawSurface::readMagic( std::istream & file )
{
    read( file, magic );

    if ( !isDirectDrawSurface( magic ))
        throw InvalidType();
}

void DirectDrawSurface::readHeader( std::istream & file )
{
    read( file, header );
}

void DirectDrawSurface::readSurfaceData( std::istream & file )
{
    surface.resize( countSurfaceBlocks() );

    for ( auto first( surface.begin() ), last( surface.end() ); first != last; ++first )
    {
        readReferenceColors( file, *first );
        read( file, *++first );
    }
}

uint32_t DirectDrawSurface::countSurfaceBlocks() const
{
    return header.width / 4 * header.height / 4 * 2;
}

void DirectDrawSurface::writeMagic( std::ostream & file ) const
{
    if ( !isDirectDrawSurface( magic ))
        throw InvalidType();

    write( file, magic );
}

void DirectDrawSurface::writeHeader( std::ostream & file ) const
{
    write( file, header );
}

void DirectDrawSurface::writeSurfaceData( std::ostream & file ) const
{
    for ( auto first( surface.begin() ), last( surface.end() ); first != last; ++first )
    {
        writeReferenceColors( file, *first );
        write( file, *++first );
    }
}
