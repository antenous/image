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
    }
    catch ( const std::istream::failure & )
    {
        throw BadFile();
    }
}

void DirectDrawSurface::readMagic( std::istream & file )
{
    file.read( reinterpret_cast< char* >( &magic ), sizeof( magic ));

    if ( !isDirectDrawSurface( magic ))
        throw InvalidType();
}

void DirectDrawSurface::readHeader( std::istream & file )
{
    file.read( reinterpret_cast< char* >( &header ), sizeof( header ));
}
