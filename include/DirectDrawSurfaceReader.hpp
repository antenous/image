/*
 * DirectDrawSurfaceReader.hpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTDRAWSURFACEREADER_HPP_
#define DIRECTDRAWSURFACEREADER_HPP_

#include "DirectDrawSurface.hpp"

namespace image
{

    class DirectDrawSurfaceReader
    {
    public:
        /**
            Read a direct draw surface image from the stream

            @throw BadFile if unable to read from the stream
            @throw InvalidType if the stream does not contain a direct draw surface image
        */
        static DirectDrawSurface read(std::istream && from);
    };

}

#endif
