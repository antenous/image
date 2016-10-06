/*
 * BitmapConverter.hpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#ifndef BITMAPCONVERTER_HPP_
#define BITMAPCONVERTER_HPP_

#include <array>
#include <cstdint>
#include <vector>
#include "Bitmap.hpp"
#include "DirectDrawSurface.hpp"

namespace image
{

    class BitmapConverter
    {
    public:
        class BadSize;

        typedef std::array< uint16_t, 4 > Color;

        typedef std::array< uint16_t, 16 > Block;

        typedef std::vector< uint16_t > Palette;

        typedef std::vector< uint32_t > Converted;

        Converted convert( const Bitmap & bmp ) const;

        Converted convert( const DirectDrawSurface & dds ) const;

    protected:
        Palette trueToHigh( const Bitmap::Palette & palette ) const;

        uint16_t trueToHigh( uint32_t trueColor ) const;

        Palette rearrangePaletteToBlocks( int32_t height, int32_t width, const Palette & palette ) const;

        Converted convertBlocks( const Palette & blocks ) const;

        std::array< uint32_t, 2 > convertBlock( const Block & block ) const;

        Color createColorTable( const Block & block ) const;

        uint32_t createLookupTable( const Color & color, const Block & block ) const;

        Palette rearrangeBlocksToPalette( int32_t height, int32_t width, const Palette & palette ) const;

        Color createColorTable( uint32_t referenceColors ) const;

        Palette createColorBlocks( const DirectDrawSurface::Surface & surface ) const;

        Block createColorBlock( const Color & color, uint32_t lookupTable ) const;

        Converted highToTrue( const Palette & palette ) const;

        uint32_t highToTrue( uint16_t highColor ) const;
    };

    class BitmapConverter::BadSize : public std::runtime_error
    {
    public:
        BadSize() :
            std::runtime_error( "bad size" )
        {}
    };
}

#endif
