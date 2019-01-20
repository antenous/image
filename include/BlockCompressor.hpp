/*
 * @file include/BlockCompressor.hpp
 * BlockCompressor.hpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#ifndef BLOCKCOMPRESSOR_HPP_
#define BLOCKCOMPRESSOR_HPP_

#include "Bitmap.hpp"
#include "DirectDrawSurface.hpp"

namespace image
{

    /**
        @brief  Class for compressing and decompressing image data
    */
    class BlockCompressor
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
        static DirectDrawSurface::Data compress(const Bitmap::Colors & in, int32_t height, int32_t width);

        /**
            @brief Decompress data in direct draw surface image

            @param in           Data to decompress
            @param height       Image height
            @param width        Image width
            @return decompressed colors for bitmap image
        */
        static Bitmap::Colors decompress(const DirectDrawSurface::Data & in, int32_t height, int32_t width);

    private:
        /**
            @brief Compress high (16bit) colors in direct draw surface ordering

            @param in   Colors to compress
            @return compressed data for direct draw surface

            @throw BadSize if color range size is not a multiple of 16
        */
        static DirectDrawSurface::Data compress(const std::vector<HighColor> & in);

        /**
            @brief Decompress data in direct draw surface image

            @param in Data to decompress
            @return decompressed high (16bit) colors in direct draw surface ordering

            @throw BadSize if data size is not a multiple of 2
        */
        static std::vector<HighColor> decompress(const DirectDrawSurface::Data & in);
    };

    class BlockCompressor::BadSize : public std::invalid_argument
    {
    public:
        BadSize();
    };

}

#endif
