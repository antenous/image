/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <istream>

using namespace image;

namespace
{
    template< typename T >
    void read( std::istream & file, T & t )
    {
        file.read( reinterpret_cast< char* >( &t ), sizeof( t ));
    }

    bool isBitmap( char type[2] )
    {
        return type[0] == 'B' && type[1] == 'M';
    }
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
    }
    catch ( const std::istream::failure & )
    {
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
