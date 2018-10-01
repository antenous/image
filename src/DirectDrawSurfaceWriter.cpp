/*
 * DirectDrawSurfaceWriter.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceWriter.hpp"
#include "Writer.hpp"

using namespace image;

namespace
{
    void write(std::ostream & to, DirectDrawSurface::Magic magic)
    {
        Writer::write(to, magic);
    }

    void write(std::ostream & to, const DirectDrawSurface::Header & header)
    {
        Writer::write(to, header);
    }

    void write(std::ostream & to, const DirectDrawSurface::Data & data, uint32_t size)
    {
        to.write(reinterpret_cast<const char*>(data.data()), size);
    }


    void writeDirectDrawSurface(std::ostream & to, const DirectDrawSurface & dds)
    {
        write(to, dds.magic);
        write(to, dds.header);
        write(to, dds.data, dds.header.pitch);
    }
}

DirectDrawSurfaceWriter::BadFile::BadFile() :
    std::invalid_argument("bad file")
{}

DirectDrawSurfaceWriter::InvalidType::InvalidType() :
    std::invalid_argument("invalid type")
{}

void DirectDrawSurfaceWriter::write(std::ostream && to, const DirectDrawSurface & dds)
{
    if (!to)
        throw BadFile();

    if (!dds)
        throw InvalidType();

    writeDirectDrawSurface(to, dds);
}
