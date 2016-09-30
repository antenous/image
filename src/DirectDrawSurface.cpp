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

    template< typename T >
    void read( std::istream & file, T & t )
    {
        file.read( reinterpret_cast< char* >( &t ), sizeof( t ));
    }

    uint32_t readSurfaceBlock( std::istream & file )
    {
        uint32_t block;
        read( file, block );
        return block;
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
        surfaceBlock = readSurfaceBlock( file );
}

uint32_t DirectDrawSurface::countSurfaceBlocks() const
{
    return header.width / 4 * header.height / 4 * 2;
}
