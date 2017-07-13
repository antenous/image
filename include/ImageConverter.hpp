/*
 * @file include/ImageConverter.hpp
 * ImageConverter.hpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#ifndef IMAGECONVERTER_HPP_
#define IMAGECONVERTER_HPP_

#include <cstdint>
#include <vector>
#include "Bitmap.hpp"
#include "DirectDrawSurface.hpp"

namespace image
{

    /**
        @brief  Class for running conversions between image types

        This class is used to run conversions between different image types.
        The current supported conversion is between a bitmap image and a direct
        draw surface image.
    */
    class ImageConverter
    {
    public:
        class BadBitmap;

        class BadDirectDrawSurface;

        /**
            Convert a bitmap into a direct draw surface

            @param bmp  Bitmap image to be converted

            @throw BadBitmap if the bmp is not a valid image
            @throw BlockCompressor::BadSize if the image has unsupported dimensions
        */
        static DirectDrawSurface convert( const Bitmap & bmp );

        /**
            Convert a direct draw surface into a bitmap

            @param dds  Direct draw surface to be converted

            @throw BadDirectDrawSurface if the dds is not a valid image
            @throw BlockCompressor::BadSize if the image has unsupported dimensions
        */
        static Bitmap convert( const DirectDrawSurface & dds );

    private:
        static void convertData( DirectDrawSurface & dds, const Bitmap & bmp );

        static void createFileHeader( DirectDrawSurface & dds, const Bitmap & bmp );

        static void convertData( Bitmap & bmp, const DirectDrawSurface & dds );

        static void createInfoHeader( Bitmap & bmp, const DirectDrawSurface & dds );

        static void createFileHeader( Bitmap & bmp );
    };

    class ImageConverter::BadBitmap : public std::runtime_error
    {
    public:
        BadBitmap() :
            std::runtime_error( "bad bitmap" )
        {}
    };

    class ImageConverter::BadDirectDrawSurface : public std::runtime_error
    {
    public:
        BadDirectDrawSurface() :
            std::runtime_error( "bad direct draw surface" )
        {}
    };

}

#endif
