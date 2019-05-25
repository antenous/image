/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"

using namespace image;

Bitmap::operator bool() const
{
    return fileHeader.type[0] == 'B' && fileHeader.type[1] == 'M';
}

int32_t Bitmap::height() const
{
    return infoHeader.height;
}

int32_t Bitmap::width() const
{
    return infoHeader.width;
}

uint8_t Bitmap::padding() const
{
    return infoHeader.padding();
}

uint8_t Bitmap::InfoHeader::padding() const
{
    const auto bytesInRow((bits*width + 31)/32*4);
    return bytesInRow - width*3;
}
