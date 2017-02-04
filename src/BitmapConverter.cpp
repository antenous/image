/*
 * BitmapConverter.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "BitmapConverter.hpp"
#include "ColorDepth.hpp"
#include "ColorPalette.hpp"
#include "BlockCompressor.hpp"

using namespace image;

BitmapConverter::Converted BitmapConverter::convert( const Bitmap & bitmap ) const
{
    return BlockCompressor::compress(
           ColorPalette::rearrangeForDirectDrawSurface( bitmap.getHeight(), bitmap.getWidth(),
           ColorDepth::trueToHigh( bitmap.getPalette() )));
}

BitmapConverter::Converted BitmapConverter::convert( const DirectDrawSurface & dds ) const
{
    return ColorDepth::highToTrue(
           ColorPalette::rearrangeForBitmap( dds.getHeight(), dds.getWidth(),
           BlockCompressor::decompress( dds.getSurface() )));
}
