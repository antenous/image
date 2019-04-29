/*
 * DirectDrawSurface.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include "DirectX.hpp"

using namespace image;

DirectDrawSurface::operator bool() const
{
    return magic == DirectX::DDS_MAGIC;
}

uint32_t DirectDrawSurface::height() const
{
    return header.height;
}

uint32_t DirectDrawSurface::width() const
{
    return header.width;
}
