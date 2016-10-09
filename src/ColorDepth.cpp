/*
 * ColorDepth.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorDepth.hpp"
#include <initializer_list>

using namespace image;

namespace
{
    uint8_t downscaleSubColor( uint32_t color, uint8_t bits )
    {
        return ( color & 0xff ) >> ( 8 - bits );
    }

    uint8_t upscaleSubColor( uint16_t color, uint8_t bits )
    {
        const uint8_t subColor(( color << ( 8 - bits )) & 0xff );
        return subColor | ( subColor >> bits );
    }
}

uint16_t ColorDepth::trueToHigh( uint32_t trueColor )
{
    uint16_t highColor( 0 );

    for ( auto bitsToKeep : { 5, 6, 5 })
    {
        highColor <<= bitsToKeep;
        highColor |= downscaleSubColor( trueColor, bitsToKeep );
        trueColor >>= 8;
    }

    return highColor;
}

uint32_t ColorDepth::highToTrue( uint16_t highColor )
{
    uint32_t trueColor( 0 );

    for ( auto bitsToKeep : { 5, 6, 5 })
    {
        trueColor <<= 8;
        trueColor |= upscaleSubColor( highColor, bitsToKeep );
        highColor >>= bitsToKeep;
    }

    return trueColor;
}
