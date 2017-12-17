/*
 * BitmapWriter.hpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#ifndef BITMAPWRITER_HPP_
#define BITMAPWRITER_HPP_

#include "Bitmap.hpp"

namespace image
{

    class BitmapWriter
    {
    public:
        /**
            Write the bitmap image to the stream

            @throw BadFile if unable to write to the stream
            @throw InvalidType if the image is not a valid bitmap image
        */
        static void write(std::ostream && to, const Bitmap & bmp);
    };

}

#endif
