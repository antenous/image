/*
 * BitmapWriter.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapWriter.hpp"

using namespace image;

void BitmapWriter::write(std::ostream && to, const Bitmap & bmp)
{
    bmp.save(to);
}
