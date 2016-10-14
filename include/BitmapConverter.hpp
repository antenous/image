/*
 * @file include/BitmapConverter.hpp
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

    /**
        @brief  Class for running conversions between image types

        This class is used to run conversions between different image types.
        The current supported conversion is between a bitmap image and a direct
        draw surface image.
    */
    class BitmapConverter
    {
    public:
        class BadSize;

        typedef std::array< uint16_t, 4 > Color;

        typedef std::array< uint16_t, 16 > Block;

        typedef std::vector< uint16_t > HighColorPalette;

        typedef std::vector< uint32_t > Converted;

        /**
            Convert a bitmap into a direct draw surface

            @param bmp  Bitmap image to be converted
            @return Surface data for the direct draw surface image

            @throw BadSize if the image has unsupported dimensions
        */
        Converted convert( const Bitmap & bmp ) const;

        /**
            Convert a direct draw surface into a bitmap

            @param dds  Direct draw surface to be converted
            @return Color palette for the bitmap image

            @throw BadSize if the image has unsupported dimensions
        */
        Converted convert( const DirectDrawSurface & dds ) const;

    protected:
        Converted convertBlocks( const HighColorPalette & blocks ) const;

        std::array< uint32_t, 2 > convertBlock( const Block & block ) const;

        Color createColorTable( const Block & block ) const;

        uint32_t createLookupTable( const Color & color, const Block & block ) const;

        Color createColorTable( uint32_t referenceColors ) const;

        HighColorPalette createColorBlocks( const DirectDrawSurface::Surface & surface ) const;

        Block createColorBlock( const Color & color, uint32_t lookupTable ) const;
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
