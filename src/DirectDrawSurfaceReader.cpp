/*
 * DirectDrawSurfaceReader.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceReader.hpp"

using namespace image;

namespace
{
    template<typename T>
    void read(std::istream & stream, T & t, std::streamsize count = sizeof(T))
    {
        stream.read(reinterpret_cast<char*>(&t), count);
    }

    void read(std::istream & from, DirectDrawSurface::Data & data, uint32_t size)
    {
        data.resize(size/sizeof(DirectDrawSurface::Data::value_type));
        read(from, *data.data(), size);
    }

    DirectDrawSurface readDirectDrawSurface(std::istream & from)
    {
        DirectDrawSurface dds;
        read(from, dds.magic);

        if (!dds)
            throw DirectDrawSurfaceReader::InvalidType();

        read(from, dds.header);
        read(from, dds.data, dds.header.pitch);

        return dds;
    }
}

DirectDrawSurfaceReader::BadFile::BadFile() :
    std::invalid_argument("bad file")
{}

DirectDrawSurfaceReader::InvalidType::InvalidType() :
    std::runtime_error("invalid type")
{}

DirectDrawSurface DirectDrawSurfaceReader::read(std::istream & from)
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
