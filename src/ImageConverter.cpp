/*
 * ImageConverter.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "ImageConverter.hpp"
#include "ColorDepth.hpp"
#include "ColorPalette.hpp"
#include "BlockCompressor.hpp"

using namespace image;

ImageConverter::Converted ImageConverter::convert( const Bitmap & bitmap ) const
{
    return BlockCompressor::compress(
           ColorPalette::rearrangeForDirectDrawSurface( bitmap.getHeight(), bitmap.getWidth(),
           ColorDepth::trueToHigh( bitmap.getPalette() )));
}

ImageConverter::Converted ImageConverter::convert( const DirectDrawSurface & dds ) const
{
    return ColorDepth::highToTrue(
           ColorPalette::rearrangeForBitmap( dds.getHeight(), dds.getWidth(),
           BlockCompressor::decompress( dds.getSurface() )));
}
