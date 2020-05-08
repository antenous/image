/*
 * ImageConverter.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "ImageConverter.hpp"
#include "BitmapCompressor.hpp"
#include "DirectDrawSurfaceDecompressor.hpp"
#include "DirectX.hpp"

using namespace image;

ImageConverter::BadBitmap::BadBitmap() :
    std::runtime_error("bad bitmap")
{}

ImageConverter::BadDirectDrawSurface::BadDirectDrawSurface() :
    std::runtime_error("bad direct draw surface")
{}

namespace
{
    DirectDrawSurface::Header createFileHeader(const Bitmap & bmp)
    {
        DirectDrawSurface::Header header{};

        header.size = sizeof(DirectDrawSurface::Header);
        header.flags = DirectX::DDS_HEADER_FLAGS_TEXTURE;
        header.height = bmp.height();
        header.width = bmp.width();
        header.pitch = header.height*header.width*Texel::size()/Texel::pixels();
        header.caps = DirectX::DDS_SURFACE_FLAGS_TEXTURE;
        header.pixelFormat.size = sizeof(DirectDrawSurface::Header::PixelFormat);
        header.pixelFormat.flags = DirectX::DDS_FOURCC;
        header.pixelFormat.fourCC = DirectX::DDS_DXT1;

        return header;
    }
}

DirectDrawSurface ImageConverter::convert(const Bitmap & bmp)
{
    if (!bmp)
        throw BadBitmap();

    DirectDrawSurface dds{};
    dds.magic = DirectX::DDS_MAGIC;
    dds.data = BitmapCompressor::compress(bmp.colors, bmp.height(), bmp.width());
    dds.header = createFileHeader(bmp);

    return dds;
}

Bitmap ImageConverter::convert(const DirectDrawSurface & dds)
{
    if (!dds)
        throw BadDirectDrawSurface();

    return Bitmap::make(dds.height(), dds.width(),
        DirectDrawSurfaceDecompressor::decompress(dds.data, dds.height(), dds.width()));
}
