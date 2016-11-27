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
        std::cout << "usage: " << name << " <bmp|dds file>" << std::endl;
        return 0;
    }

    template< typename T >
    T load( const char * file )
    {
        std::ifstream in( file, std::ios::binary );
        T t;
        t.loadFrom( in );
        return t;
    }

    template< typename T, typename U >
    T convertTo( const U & u )
    {
        T t;
        t.convertFrom( u );
        return t;
    }

    void writeToFile( const DirectDrawSurface & dds )
    {
        std::ofstream bkp( "out.dds", std::ios::binary );
        dds.saveTo( bkp );
    }

    void writeToFile( const Bitmap & bmp )
    {
        std::ofstream out( "out.bmp", std::ios::binary );
        bmp.saveTo( out );
    }
}

int main( int argc, char * argv[] )
{
    if ( argc != 2 )
        return help( argv[0] );

    try
    {
        writeToFile( convertTo< DirectDrawSurface >( load< Bitmap >( argv[1] )));
    }
    catch ( const Bitmap::InvalidType & )
    {
        try
        {
            writeToFile( convertTo< Bitmap >( load< DirectDrawSurface >( argv[1] )));
        }
        catch ( const std::runtime_error & e )
        {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    catch( const std::runtime_error & e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
