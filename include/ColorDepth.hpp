/*
 * @file include/ColorDepth.hpp
 * ColorDepth.hpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#ifndef COLORDEPTH_HPP_
#define COLORDEPTH_HPP_

#include <cstdint>
#include <vector>
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
            @brief Convert a 24bit color range into a 16bit color range

            @param in   24bit color range to convert
            @return 16bit color range
        */
        static std::vector<HighColor> trueToHigh(const std::vector<TrueColor> & in);

        /**
            @brief Convert a 16bit color range into a 24bit color range

            @param in   16bit color range to convert
            @return 24bit color range
        */
        static std::vector<TrueColor> highToTrue(const std::vector<HighColor> & in);
    };

}

#endif
