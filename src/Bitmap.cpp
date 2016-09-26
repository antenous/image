/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"

using namespace image;

namespace
{
    bool isEmpty( std::istream & file )
    {
        return file.peek() == std::istream::traits_type::eof();
    }

    bool hasFileHeader( std::istream & file )
    {
        auto pos( file.tellg() );
        file.seekg( 0, std::ios::end );
        auto end( file.tellg() );
        file.seekg( pos );
        return end - pos >= 14;
    }

    template< typename T >
    void read( std::istream & file, T & t )
    {
        file.read( reinterpret_cast< char* >( &t ), sizeof( t ));
    }

    bool isBitmap( char type[2] )
    {
        return type[0] != 'B' && type[1] != 'M';
    }
}

void Bitmap::read( std::istream & file )
{
    readFileHeader( file );
    readInfoHeader( file );
}

void Bitmap::readFileHeader( std::istream & file )
{
    if ( !file || isEmpty( file ) || !hasFileHeader( file ))
        throw BadFile();

    ::read( file, fileHeader.type );
    ::read( file, fileHeader.size );
    ::read( file, fileHeader.reserved1 );
    ::read( file, fileHeader.reserved2 );
    ::read( file, fileHeader.offset );

    if ( isBitmap( fileHeader.type ))
        throw InvalidType();
}

void Bitmap::readInfoHeader( std::istream & file )
{
    ::read( file, infoHeader.size );

    if ( infoHeader.size != 40 )
        throw UnknownInfoHeader();

    ::read( file, infoHeader.width );
    ::read( file, infoHeader.height );
    ::read( file, infoHeader.planes );
    ::read( file, infoHeader.bits );
    ::read( file, infoHeader.compression );
    ::read( file, infoHeader.imageSize );
    ::read( file, infoHeader.horizontalResolution );
    ::read( file, infoHeader.verticalResolution );
    ::read( file, infoHeader.colors );
    ::read( file, infoHeader.importantColors );
}
