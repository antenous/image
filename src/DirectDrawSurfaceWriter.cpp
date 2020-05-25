/*
 * DirectDrawSurfaceWriter.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceWriter.hpp"

using namespace image;

namespace
{
    template<typename T>
    void write(std::ostream & stream, const T & t, std::streamsize count = sizeof(T))
    {
        stream.write(reinterpret_cast<const char*>(&t), count);
    }

    void write(std::ostream & to, const DirectDrawSurface::Data & data, uint32_t size)
    {
        write(to, *data.data(), size);
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

void DirectDrawSurfaceWriter::write(std::ostream & to, const DirectDrawSurface & dds)
{
    if (!to)
        throw BadFile();

    if (!dds)
        throw InvalidType();

    writeDirectDrawSurface(to, dds);
}
