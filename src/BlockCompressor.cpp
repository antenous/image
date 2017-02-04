/*
 * BlockCompressor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#include "BlockCompressor.hpp"

using namespace image;

namespace
{
    uint8_t red( uint16_t color )
    {
        return color >> 11;
    }

    uint16_t red( uint8_t color )
    {
        return color << 11;
    }

    uint8_t green( uint16_t color )
    {
        return ( color >> 5 ) & 0x3f;
    }

    uint16_t green( uint8_t color )
    {
        return ( color & 0x3f ) << 5;
    }

    uint8_t blue( uint16_t color )
    {
        return color & 0x1f;
    }

    uint16_t blue( uint8_t color )
    {
        return color & 0x1f;
    }

    uint8_t interpolate( uint8_t lhs, uint8_t rhs )
    {
        return 2*lhs/3 + rhs/3;
    }
}

uint16_t BlockCompressor::interpolate( uint16_t lhs, uint16_t rhs )
{
    return
        red(   ::interpolate( red(   lhs ), red(   rhs ))) +
        green( ::interpolate( green( lhs ), green( rhs ))) +
        blue(  ::interpolate( blue(  lhs ), blue(  rhs )));
}

uint32_t BlockCompressor::referenceColors( const Color & color )
{
    return color[0] << 16 | color[1];
}

uint8_t BlockCompressor::findClosest( Color color, uint16_t ref )
{
    for ( auto & c : color )
        c = std::abs( c - ref );

    return std::distance( color.begin(), std::min_element( color.begin(), color.end() ));
}

BlockCompressor::Color BlockCompressor::createColorTable( uint32_t referenceColors )
{
    Color color;
    color[0] = referenceColors >> 16;
    color[1] = referenceColors & 0xffff;
    color[2] = interpolate( color[0], color[1] );
    color[3] = interpolate( color[1], color[0] );

    return color;
}

uint32_t BlockCompressor::reorderBytes( uint32_t lookup )
{
    return ( lookup >> 24 ) +
          (( lookup >> 8 ) & 0xff00 ) +
          (( lookup & 0xff00 ) << 8 ) +
           ( lookup << 24 );
}
