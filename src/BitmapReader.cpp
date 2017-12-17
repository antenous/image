/*
 * BitmapReader.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapReader.hpp"

using namespace image;

Bitmap BitmapReader::read(std::istream && from)
{
    Bitmap bmp;
    bmp.load(from);
    return bmp;
}
