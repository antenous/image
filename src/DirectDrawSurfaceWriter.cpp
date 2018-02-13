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

    void write(std::ostream & to, DirectDrawSurface::Data && data, uint32_t size)
    {
        to.write(reinterpret_cast<const char*>(data.data()), size);
    }

    void rearrangeReferenceColors(uint32_t & colors)
    {
        colors = (colors << 16)|(colors >> 16);
    }

    auto rearrangeReferenceColors(const DirectDrawSurface::Data & data)
    {
        auto rearrange(data);

        for (auto first(rearrange.begin()), last(rearrange.end()); first != last; std::advance(first, 2))
            rearrangeReferenceColors(*first);

        return rearrange;
    }

    void writeDirectDrawSurface(std::ostream & to, const DirectDrawSurface & dds)
    {
        write(to, dds.magic);
        write(to, dds.header);
        write(to, rearrangeReferenceColors(dds.data), dds.header.pitch);
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
