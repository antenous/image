/*
 * @file include/BlockCompressor.hpp
 * BlockCompressor.hpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#ifndef BLOCKCOMPRESSOR_HPP_
#define BLOCKCOMPRESSOR_HPP_

#include <stdexcept>
#include <vector>

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
            @brief Compress a color range

            @param in   Color range to compress
            @return compressed color range

            @throw BadSize if color range size is not a multiple of 16
        */
        static std::vector<uint32_t> compress(const std::vector<uint16_t> & in);

        /**
            @brief Decompress a color range

            @param in Color range to compress
            @return decompressed color range

            @throw BadSize if color range size is not a multiple of 2
        */
        static std::vector<uint16_t> decompress(const std::vector<uint32_t> & in);
    };

    class BlockCompressor::BadSize : public std::invalid_argument
    {
    public:
        BadSize();
    };

}

#endif
