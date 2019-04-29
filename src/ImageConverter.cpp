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

DirectDrawSurface ImageConverter::convert( const Bitmap & bmp )
{
    if ( !bmp )
        throw BadBitmap();

    DirectDrawSurface dds{};
    convertData( dds, bmp );
    createFileHeader( dds, bmp );

    return dds;
}

void ImageConverter::convertData( DirectDrawSurface & dds, const Bitmap & bmp )
{
    dds.data = BlockCompressor::compress(bmp.colors, bmp.height(), bmp.width());
}

void ImageConverter::createFileHeader( DirectDrawSurface & dds, const Bitmap & bmp )
{
    dds.magic = DirectX::DDS_MAGIC;
    dds.header.size = sizeof(DirectDrawSurface::Header);
    dds.header.flags = DirectX::DDS_HEADER_FLAGS_TEXTURE;
    dds.header.height = bmp.height();
    dds.header.width = bmp.width();
    dds.header.pitch = dds.data.size() * sizeof(DirectDrawSurface::Data::value_type);
    dds.header.caps = DirectX::DDS_SURFACE_FLAGS_TEXTURE;

    dds.header.pixelFormat.size = sizeof(DirectDrawSurface::Header::PixelFormat);
    dds.header.pixelFormat.flags = DirectX::DDS_FOURCC;
    dds.header.pixelFormat.fourCC = DirectX::DDS_DXT1;
}

Bitmap ImageConverter::convert( const DirectDrawSurface & dds )
{
    if ( !dds )
        throw BadDirectDrawSurface();

    Bitmap bmp{};
    createInfoHeader( bmp, dds );
    createFileHeader( bmp );
    convertData( bmp, dds );

    return bmp;
}

void ImageConverter::convertData( Bitmap & bmp, const DirectDrawSurface & dds )
{
    bmp.colors = BlockCompressor::decompress(dds.data, dds.height(), dds.width());
}

void ImageConverter::createInfoHeader( Bitmap & bmp, const DirectDrawSurface & dds )
{
    bmp.infoHeader.size = 40;
    bmp.infoHeader.width = dds.width();
    bmp.infoHeader.height = dds.height();
    bmp.infoHeader.planes = 1;
    bmp.infoHeader.bits = 24;
    bmp.infoHeader.imageSize = (3*dds.width() + bmp.padding())*dds.height();
}

void ImageConverter::createFileHeader( Bitmap & bmp )
{
    bmp.fileHeader.type[0] = 'B';
    bmp.fileHeader.type[1] = 'M';
    bmp.fileHeader.size = 14;
    bmp.fileHeader.offset = 54;
    bmp.fileHeader.size += bmp.infoHeader.size + bmp.infoHeader.imageSize;
}
