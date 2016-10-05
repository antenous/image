/*
 * DirectDrawSurface.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include <cstring>
#include <istream>
#include "Bitmap.hpp"
#include "BitmapConverter.hpp"

using namespace image;

namespace
{
    bool isDirectDrawSurface( uint32_t magic )
    {
        return magic == 0x20534444;
    }

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
}

void DirectDrawSurface::loadFrom( std::istream & file )
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
        throw BadFile();
    }
}

void DirectDrawSurface::convertFrom( const Bitmap & bitmap )
{
    if ( !bitmap )
        throw BadBitmap();

    memset( &header, 0, sizeof( header ));

    magic = 0x20534444;
    header.size = 124;
    header.flags = 0x1 | 0x2 | 0x4 | 0x1000;
    header.height = bitmap.getHeight();
    header.width = bitmap.getWidth();
    header.caps = 0x1000;

    header.pixelFormat.size = 32;
    header.pixelFormat.flags = 0x4;
    header.pixelFormat.fourCC[0] = 'D';
    header.pixelFormat.fourCC[1] = 'X';
    header.pixelFormat.fourCC[2] = 'T';
    header.pixelFormat.fourCC[3] = '1';

    surfaceBlocks = BitmapConverter().convert( bitmap );

    header.pitch = surfaceBlocks.size() * 4;
}

void DirectDrawSurface::saveTo( std::ostream & file ) const
{
    if ( !file )
        throw BadFile();

    writeMagic( file );
    writeHeader( file );
    writeSurfaceData( file );
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
    surfaceBlocks.resize( countSurfaceBlocks() );

    for ( auto & surfaceBlock : surfaceBlocks )
        read( file, surfaceBlock );
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
    for ( auto surfaceBlock : surfaceBlocks )
        write( file, surfaceBlock );
}
