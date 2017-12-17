/*
 * DirectDrawSurfaceReader.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceReader.hpp"

using namespace image;

DirectDrawSurface DirectDrawSurfaceReader::read(std::istream && from)
{
    DirectDrawSurface dds;
    dds.load(from);
    return dds;
}
