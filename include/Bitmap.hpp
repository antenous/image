/*
 * @file include/Bitmap.hpp
 * Bitmap.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#ifndef BITMAP_HPP_
#define BITMAP_HPP_

#include <vector>
#include "TrueColor.hpp"

namespace image
{

    /**
        Class to hold all the data about a bitmap image
    */
    class Bitmap
    {
    public:
        using Colors = std::vector<TrueColor>;

        using Magic = std::uint16_t;

        /**
            Return true if bitmap is valid
        */
        explicit operator bool() const;

        /**
            Create a new bitmap
        */
        static Bitmap make(std::int32_t height, std::int32_t width, const Colors & colors);

        /**
            Return height of the image
        */
        int32_t height() const;

        /**
            Return width of the image
        */
        int32_t width() const;

        /**
            Return the number of padding bytes in a row
        */
        uint8_t padding() const;

        Magic magic = 0;

        struct FileHeader
        {
            uint32_t size;
            uint32_t reserved;
            uint32_t offset;
        } fileHeader{};

        struct InfoHeader
        {
            /**
                Return the number of padding bytes in a row
            */
            uint8_t padding() const;

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
        } infoHeader{};

        Colors colors;
    };

}

#endif
