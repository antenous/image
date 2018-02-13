/*
 * DirectDrawSurface.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"

using namespace image;

DirectDrawSurface::operator bool() const
{
    return magic == 0x20534444;
}

uint32_t DirectDrawSurface::height() const
{
    return header.height;
}

uint32_t DirectDrawSurface::width() const
{
    return header.width;
}
