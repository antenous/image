/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <ostream>

using namespace image;

namespace
{
    bool isBitmap( const char type[2] )
    {
        return type[0] == 'B' && type[1] == 'M';
    }
}

Bitmap::operator bool() const
{
    return isBitmap( fileHeader.type );
}

int32_t Bitmap::getHeight() const
{
    return infoHeader.height;
}

int32_t Bitmap::getWidth() const
{
    return infoHeader.width;
}

Bitmap::Colors Bitmap::getColors() const
{
    return colors;
}

uint8_t Bitmap::padding() const
{
    const auto bytesInRow(( infoHeader.bits * infoHeader.width + 31 ) / 32 * 4 );
    return bytesInRow - infoHeader.width * 3;
}
