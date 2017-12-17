/*
 * DirectDrawSurfaceWriter.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceWriter.hpp"

using namespace image;

void DirectDrawSurfaceWriter::write(std::ostream && to, const DirectDrawSurface & dds)
{
    dds.save(to);
}
