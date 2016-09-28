/*
 * main.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: Antero Nousiainen
 */

#include <fstream>
#include <iostream>
#include "Bitmap.hpp"

using namespace image;

namespace
{
    int help( const char * name )
    {
        std::cout << "usage: " << name << " <bitmap file>" << std::endl;
        return 0;
    }
}

int main( int argc, char * argv[] )
{
    if ( argc != 2 )
        return help( argv[0] );

    Bitmap bitmap;

    try
    {
        std::ifstream bmp( argv[1], std::ios::binary );
        bitmap.loadFrom( bmp );
        bmp.close();
    }
    catch ( const std::runtime_error & e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::ofstream bkp( "bkp.bmp", std::ios::binary );
    bitmap.saveTo( bkp );
    bkp.close();

    return 0;
}
