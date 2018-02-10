/*
 * DirectDrawSurfaceReader.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceReader.hpp"
#include "Reader.hpp"

using namespace image;

namespace
{
    void read(std::istream & from, uint32_t & magic)
    {
        Reader::read(from, magic);
    }

    void read(std::istream & from, decltype(DirectDrawSurface::header) & header)
    {
        Reader::read(from, header);
    }

    void read(std::istream & from, decltype(DirectDrawSurface::surface) & data, uint32_t size)
    {
        data.resize(size/4);
        from.read(reinterpret_cast<char*>(data.data()), size);
    }

    void rearrangeReferenceColors(uint32_t & colors)
    {
        colors = (colors << 16)|(colors >> 16);
    }

    void rearrangeReferenceColors(decltype(DirectDrawSurface::surface) & data)
    {
        for (auto first(data.begin()), last(data.end()); first != last; std::advance(first, 2))
            rearrangeReferenceColors(*first);
    }

    DirectDrawSurface readDirectDrawSurface(std::istream & from)
    {
        DirectDrawSurface dds;
        read(from, dds.magic);

        if (!dds)
            throw DirectDrawSurfaceReader::InvalidType();

        read(from, dds.header);
        read(from, dds.surface, dds.header.pitch);
        rearrangeReferenceColors(dds.surface);

        return dds;
    }
}

DirectDrawSurfaceReader::BadFile::BadFile() :
    std::invalid_argument("bad file")
{}

DirectDrawSurfaceReader::InvalidType::InvalidType() :
    std::runtime_error( "invalid type" )
{}

DirectDrawSurface DirectDrawSurfaceReader::read(std::istream && from)
{
    if (!from)
        throw BadFile();

    from.exceptions(std::istream::failbit | std::istream::badbit);

    try
    {
        return readDirectDrawSurface(from);
    }
    catch (const std::istream::failure &)
    {
        throw BadFile();
    }
}
