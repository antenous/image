/*
 * @file include/Bitmap.hpp
 * Bitmap.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#ifndef BITMAP_HPP_
#define BITMAP_HPP_

#include <stdexcept>
#include <vector>

namespace image
{

    /**
        Class to hold all the data about a bitmap image
    */
    class Bitmap
    {
    public:
        typedef std::vector< uint32_t > Colors;

        /**
            Check if holds a valid bitmap image

            @return true if holds a valid bitmap image
        */
        explicit operator bool() const;

        /**
            Return height of the image

            @return Height of the image
        */
        int32_t getHeight() const;

        /**
            Return width of the image

            @return Width of the image
        */
        int32_t getWidth() const;

        /**
            Return the image colors

            @return Image colors
        */
        Colors getColors() const;

        uint8_t padding() const;

        struct
        {
            char type[2];
            uint32_t size;
            uint32_t reserved;
            uint32_t offset;
        } fileHeader{{ 0, 0 }, 0, 0, 0 };

        struct
        {
            uint32_t size;
            int32_t width;
            int32_t height;
            uint16_t planes;
            uint16_t bits;
            uint32_t compression;
            uint32_t imageSize;
            uint32_t horizontalResolution;
            uint32_t verticalResolution;
            uint32_t colors;
            uint32_t importantColors;
        } infoHeader;

        Colors colors;
    };

}

#endif
