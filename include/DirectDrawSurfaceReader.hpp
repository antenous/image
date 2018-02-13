/*
 * @file include/DirectDrawSurfaceReader.hpp
 * DirectDrawSurfaceReader.hpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTDRAWSURFACEREADER_HPP_
#define DIRECTDRAWSURFACEREADER_HPP_

#include <stdexcept>
#include "DirectDrawSurface.hpp"

namespace image
{

    class DirectDrawSurfaceReader
    {
    public:
        class BadFile;

        class InvalidType;

        /**
            Read a direct draw surface image from the stream

            @throw BadFile if unable to read from the stream
            @throw InvalidType if the stream does not contain a direct draw surface image
        */
        static DirectDrawSurface read(std::istream && from);
    };

    class DirectDrawSurfaceReader::BadFile : public std::invalid_argument
    {
    public:
        BadFile();
    };

    class DirectDrawSurfaceReader::InvalidType : public std::runtime_error
    {
    public:
        InvalidType();
    };

}

#endif
