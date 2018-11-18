/*
 * ImageConverter.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "ImageConverter.hpp"
#include <cstring>
#include "ColorDepth.hpp"
#include "ColorPalette.hpp"
#include "BlockCompressor.hpp"

using namespace image;

DirectDrawSurface ImageConverter::convert( const Bitmap & bmp )
{
    if ( !bmp )
        throw BadBitmap();

    DirectDrawSurface dds;
    convertData( dds, bmp );
    createFileHeader( dds, bmp );

    return dds;
}

void ImageConverter::convertData( DirectDrawSurface & dds, const Bitmap & bmp )
{
    dds.data = BlockCompressor::compress(
        ColorPalette::rearrangeForDirectDrawSurface( bmp.height(), bmp.width(),
        ColorDepth::trueToHigh( bmp.colors )));
}

void ImageConverter::createFileHeader( DirectDrawSurface & dds, const Bitmap & bmp )
{
    memset( &dds.header, 0, sizeof( dds.header ));
    dds.magic = 0x20534444;
    dds.header.size = 124;
    dds.header.flags = 0x1 | 0x2 | 0x4 | 0x1000;
    dds.header.height = bmp.height();
    dds.header.width = bmp.width();
    dds.header.pitch = dds.data.size() * sizeof(DirectDrawSurface::Data::value_type);
    dds.header.caps = 0x1000;

    dds.header.pixelFormat.size = 32;
    dds.header.pixelFormat.flags = 0x4;
    dds.header.pixelFormat.fourCC[0] = 'D';
    dds.header.pixelFormat.fourCC[1] = 'X';
    dds.header.pixelFormat.fourCC[2] = 'T';
    dds.header.pixelFormat.fourCC[3] = '1';
}

Bitmap ImageConverter::convert( const DirectDrawSurface & dds )
{
    if ( !dds )
        throw BadDirectDrawSurface();

    Bitmap bmp;
    createInfoHeader( bmp, dds );
    createFileHeader( bmp );
    convertData( bmp, dds );

    return bmp;
}

void ImageConverter::convertData( Bitmap & bmp, const DirectDrawSurface & dds )
{
    bmp.colors = ColorDepth::highToTrue(
        ColorPalette::rearrangeForBitmap( dds.height(), dds.width(),
        BlockCompressor::decompress( dds.data )));
}

void ImageConverter::createInfoHeader( Bitmap & bmp, const DirectDrawSurface & dds )
{
    memset( &bmp.infoHeader, 0, sizeof( bmp.infoHeader ));

    bmp.infoHeader.size = 40;
    bmp.infoHeader.width = dds.width();
    bmp.infoHeader.height = dds.height();
    bmp.infoHeader.planes = 1;
    bmp.infoHeader.bits = 24;

    bmp.infoHeader.imageSize = (3*dds.width() + bmp.padding())*dds.height();
}

void ImageConverter::createFileHeader( Bitmap & bmp )
{
    memset( &bmp.fileHeader, 0 , sizeof( bmp.fileHeader ));

    bmp.fileHeader.type[0] = 'B';
    bmp.fileHeader.type[1] = 'M';
    bmp.fileHeader.size = 14;
    bmp.fileHeader.offset = 54;
    bmp.fileHeader.size += bmp.infoHeader.size + bmp.infoHeader.imageSize;
}
