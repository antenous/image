/*
 * ImageConverter.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "ImageConverter.hpp"
#include "DirectX.hpp"
#include "BlockCompressor.hpp"

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
    dds.data = BlockCompressor::compress(bmp.colors, bmp.height(), bmp.width());
    dds.header = createFileHeader(bmp);

    return dds;
}

namespace
{
    Bitmap::InfoHeader createInfoHeader(const DirectDrawSurface & dds)
    {
        Bitmap::InfoHeader infoHeader{};

        infoHeader.size = 40;
        infoHeader.width = dds.width();
        infoHeader.height = dds.height();
        infoHeader.planes = 1;
        infoHeader.bits = 24;
        infoHeader.imageSize = (3*dds.width() + infoHeader.padding())*dds.height();

        return infoHeader;
    }

    Bitmap::FileHeader createFileHeader(const Bitmap::InfoHeader & infoHeader)
    {
        Bitmap::FileHeader fileHeader{};

        fileHeader.type[0] = 'B';
        fileHeader.type[1] = 'M';
        fileHeader.size = 14 + infoHeader.size + infoHeader.imageSize;
        fileHeader.offset = 54;

        return fileHeader;
    }
}

Bitmap ImageConverter::convert(const DirectDrawSurface & dds)
{
    if (!dds)
        throw BadDirectDrawSurface();

    Bitmap bmp{};
    bmp.infoHeader = createInfoHeader(dds);
    bmp.fileHeader = createFileHeader(bmp.infoHeader);
    bmp.colors = BlockCompressor::decompress(dds.data, dds.height(), dds.width());

    return bmp;
}
