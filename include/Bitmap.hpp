/*
 * Bitmap.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#ifndef BITMAP_HPP_
#define BITMAP_HPP_

#include <stdexcept>
#include <istream>

namespace image
{

    class Bitmap
    {
    public:
        class BadFile;

        class InvalidType;

        class UnknownInfoHeader;

        struct FileHeader
        {
            char type[2];
            uint32_t size;
            uint16_t reserved1;
            uint16_t reserved2;
            uint32_t offset;
        };

        struct InfoHeader
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
        };

        void read( std::istream & file );

    private:
        void readFileHeader( std::istream & file );

        void readInfoHeader( std::istream & file );

        FileHeader fileHeader;
        InfoHeader infoHeader;
    };

    class Bitmap::BadFile : public std::runtime_error
    {
    public:
        BadFile() :
            std::runtime_error( "bad file" )
        {}
    };

    class Bitmap::InvalidType : public std::runtime_error
    {
    public:
        InvalidType() :
            std::runtime_error( "invalid type" )
        {}
    };

    class Bitmap::UnknownInfoHeader : public std::runtime_error
    {
    public:
        UnknownInfoHeader() :
            std::runtime_error( "unknown info header" )
        {}
    };

}

#endif
