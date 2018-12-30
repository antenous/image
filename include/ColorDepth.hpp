/*
 * @file include/ColorDepth.hpp
 * ColorDepth.hpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#ifndef COLORDEPTH_HPP_
#define COLORDEPTH_HPP_

#include "HighColor.hpp"
#include "TrueColor.hpp"

namespace image
{

    /**
        @brief  Class for converting color depths
    */
    class ColorDepth
    {
    public:
        /**
            @brief Convert a 24bit color into a 16bit color

            @param in   24bit color to convert
            @return 16bit color
        */
        static HighColor trueToHigh(const TrueColor & trueColor) noexcept;

        /**
            @brief Convert a 16bit color into a 24bit color

            @param in   16bit color to convert
            @return 24bit color
        */
        static TrueColor highToTrue(const HighColor & highColor) noexcept;
    };

}

#endif
