/*
 * @file include/BitmapConverter.hpp
 * BitmapConverter.hpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#ifndef BITMAPCONVERTER_HPP_
#define BITMAPCONVERTER_HPP_

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
        typedef std::vector< uint32_t > Converted;

        /**
            Convert a bitmap into a direct draw surface

            @param bmp  Bitmap image to be converted
            @return Surface data for the direct draw surface image

            @throw BlockCompressor::BadSize if the image has unsupported dimensions
        */
        Converted convert( const Bitmap & bmp ) const;

        /**
            Convert a direct draw surface into a bitmap

            @param dds  Direct draw surface to be converted
            @return Color palette for the bitmap image

            @throw BlockCompressor::BadSize if the image has unsupported dimensions
        */
        Converted convert( const DirectDrawSurface & dds ) const;
    };

}

#endif
