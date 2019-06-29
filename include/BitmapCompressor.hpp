/*
 * @file include/BitmapCompressor.hpp
 * BitmapCompressor.hpp
 *
 *  Created on: Jun 23, 2019
 *      Author: Antero Nousiainen
 */

#ifndef BITMAPCOMPRESSOR_HPP_
#define BITMAPCOMPRESSOR_HPP_

#include <stdexcept>
#include "Bitmap.hpp"
#include "DirectDrawSurface.hpp"

namespace image
{

    /**
        @brief  Class to compress a bitmap color data
    */
    class BitmapCompressor
    {
    public:
        class BadSize;

        /**
            @brief Compress colors in bitmap image

            @param in           Colors to compress
            @param height       Image height
            @param width        Image width
            @return compressed data for direct draw surface
        */
        static DirectDrawSurface::Data compress(
            const Bitmap::Colors & in, std::int32_t height, std::int32_t width);
    };

    class BitmapCompressor::BadSize : public std::invalid_argument
    {
    public:
        BadSize();
    };

}

#endif
