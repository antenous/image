/*
 * BitmapReader.hpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#ifndef BITMAPREADER_HPP_
#define BITMAPREADER_HPP_

#include "Bitmap.hpp"

namespace image
{

    class BitmapReader
    {
    public:
        /**
            Read a bitmap image from the stream

            @throw BadFile if unable to read from the stream
            @throw InvalidType if the stream does not contain a bitmap image
        */
        static Bitmap read(std::istream && from);
    };

}

#endif
