/*
 * @file include/ColorPalette.hpp
 * ColorPalette.hpp
 *
 *  Created on: Oct 14, 2016
 *      Author: Antero Nousiainen
 */

#ifndef COLORPALETTE_HPP_
#define COLORPALETTE_HPP_

#include <stdexcept>
#include <vector>

namespace image
{

    /**
        @brief  Class for rearranging the color palette

        Bitmap and direct draw surface view the image in a bit different ways;
        bitmap as a continuous stream of colors, direct draw surface as 4 by 4
        blocks of colors. When converting the image from one file format to
        another the color palette will need to be rearranged as part of the
        conversion.
    */
    class ColorPalette
    {
    public:
        class BadDimensions;

        typedef std::vector< uint16_t > Colors;

        /**
            @brief Rearrange the color palette for bitmap

            @param height       Height of the color palette
            @param width        Width of the color palette
            @param in           Color palette
            @return Rearranged color palette

            @throw BadDimensions if height or width is not a multiple of 4 or if color palette is not a multiple of 16
        */
        static Colors rearrangeForBitmap( int32_t height, int32_t width, const Colors & in );

        /**
            @brief Rearrange the color palette for direct draw surface

            @param height       Height of the color palette
            @param width        Width of the color palette
            @param in           Color palette
            @return Rearranged color palette

            @throw BadDimensions if height or width is not a multiple of 4 or if color palette is not a multiple of 16
        */
        static Colors rearrangeForDirectDrawSurface( int32_t height, int32_t width, const Colors & in );
    };

    class ColorPalette::BadDimensions : public std::runtime_error
    {
    public:
        BadDimensions() :
            std::runtime_error( "bad dimensions" )
        {}
    };

}

#endif
