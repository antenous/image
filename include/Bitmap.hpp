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

        struct FileHeader
        {
            char type[2];
            uint32_t size;
            uint16_t reserved1;
            uint16_t reserved2;
            uint32_t offset;
        };

        void read( std::istream & file );

    private:
        FileHeader fileHeader;
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

}

#endif
