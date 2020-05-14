/*
 * ImageConverter.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "ImageConverter.hpp"
#include "BitmapCompressor.hpp"
#include "DirectDrawSurfaceDecompressor.hpp"

using namespace image;

ImageConverter::BadBitmap::BadBitmap() :
    std::runtime_error("bad bitmap")
{}

ImageConverter::BadDirectDrawSurface::BadDirectDrawSurface() :
    std::runtime_error("bad direct draw surface")
{}

DirectDrawSurface ImageConverter::convert(const Bitmap & bmp)
{
    if (!bmp)
        throw BadBitmap();

    return DirectDrawSurface::make(bmp.height(), bmp.width(),
        BitmapCompressor::compress(bmp.colors, bmp.height(), bmp.width()));
}

Bitmap ImageConverter::convert(const DirectDrawSurface & dds)
{
    if (!dds)
        throw BadDirectDrawSurface();

    return Bitmap::make(dds.height(), dds.width(),
        DirectDrawSurfaceDecompressor::decompress(dds.data, dds.height(), dds.width()));
}
