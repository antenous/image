/*
 * DirectDrawSurface.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurface.hpp"
#include "DirectX.hpp"

using namespace image;

namespace
{
    DirectDrawSurface::Header::PixelFormat makePixelFormat()
    {
        return {
            sizeof(DirectDrawSurface::Header::PixelFormat), DirectX::DDS_FOURCC, DirectX::DDS_DXT1,
            0, 0, 0, 0, 0 };
    }

    DirectDrawSurface::Header makeHeader(std::uint32_t height, std::uint32_t width)
    {
        const auto pitch = height*width*Texel::size()/Texel::pixels();
        return {
            sizeof(DirectDrawSurface::Header), DirectX::DDS_HEADER_FLAGS_TEXTURE, height, width, pitch, 0, 1, {},
            makePixelFormat(),
            DirectX::DDS_SURFACE_FLAGS_TEXTURE, 0, 0, 0, 0 };
    }
}

DirectDrawSurface::operator bool() const
{
    return magic == DirectX::DDS_MAGIC;
}

DirectDrawSurface DirectDrawSurface::make(std::uint32_t height, std::uint32_t width, const Data & data)
{
    return { DirectX::DDS_MAGIC, makeHeader(height, width), data };
}

uint32_t DirectDrawSurface::height() const
{
    return header.height;
}

uint32_t DirectDrawSurface::width() const
{
    return header.width;
}
