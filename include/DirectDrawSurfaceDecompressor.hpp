/*
 * @file include/DirectDrawSurfaceDecompressor.hpp
 * DirectDrawSurfaceDecompressor.hpp
 *
 *  Created on: Jun 23, 2019
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTDRAWSURFACEDECOMPRESSOR_HPP_
#define DIRECTDRAWSURFACEDECOMPRESSOR_HPP_

#include <stdexcept>
#include "Bitmap.hpp"
#include "DirectDrawSurface.hpp"

namespace image
{

    /**
        @brief  Class to decompress direct draw surface texel data
    */
    class DirectDrawSurfaceDecompressor
    {
    public:
        class BadSize;

        /**
            @brief Decompress data in direct draw surface image

            @param in           Data to decompress
            @param height       Image height
            @param width        Image width
            @return decompressed colors for bitmap image
        */
        static Bitmap::Colors decompress(
            const DirectDrawSurface::Data & in, std::int32_t height, std::int32_t width);
    };

    class DirectDrawSurfaceDecompressor::BadSize : public std::invalid_argument
    {
    public:
        BadSize();
    };

}

#endif
