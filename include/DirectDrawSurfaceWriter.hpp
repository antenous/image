/*
 * @file include/DirectDrawSurfaceWriter.hpp
 * DirectDrawSurfaceWriter.hpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTDRAWSURFACEWRITER_HPP_
#define DIRECTDRAWSURFACEWRITER_HPP_

#include <ostream>
#include "DirectDrawSurface.hpp"

namespace image
{

    class DirectDrawSurfaceWriter
    {
    public:
        class BadFile;

        class InvalidType;


        /**
            Write the direct draw surface image to the stream

            @throw BadFile if unable to write to the stream
            @throw InvalidType if the image is not a valid direct draw surface image
        */
        static void write(std::ostream && to, const DirectDrawSurface & dds);
    };

    class DirectDrawSurfaceWriter::BadFile : public std::invalid_argument
    {
    public:
        BadFile();
    };

    class DirectDrawSurfaceWriter::InvalidType : public std::invalid_argument
    {
    public:
        InvalidType();
    };

}

#endif
