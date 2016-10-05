/*
 * main.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: Antero Nousiainen
 */

#include <fstream>
#include <iostream>
#include "Bitmap.hpp"
#include "DirectDrawSurface.hpp"

using namespace image;

namespace
{
    int help( const char * name )
    {
        std::cout << "usage: " << name << " <bitmap file>" << std::endl;
        return 0;
    }

    Bitmap loadBitmap( const char * file )
    {
        std::ifstream in( file, std::ios::binary );
        Bitmap bmp;
        bmp.loadFrom( in );
        return std::move( bmp );
    }

    DirectDrawSurface convertToDirectDraw( const Bitmap & bitmap )
    {
        DirectDrawSurface dds;
        dds.convertFrom( bitmap );
        return std::move( dds );
    }

    void writeToFile( const DirectDrawSurface & dds )
    {
        std::ofstream bkp( "out.dds", std::ios::binary );
        dds.saveTo( bkp );
    }
}

int main( int argc, char * argv[] )
{
    if ( argc != 2 )
        return help( argv[0] );

    try
    {
        writeToFile( convertToDirectDraw( loadBitmap( argv[1]) ));
    }
    catch ( const std::runtime_error & e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
