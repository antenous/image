/*
 * @file include/TrueColor.hpp
 * TrueColor.hpp
 *
 *  Created on: Nov 18, 2018
 *      Author: Antero Nousiainen
 */

#ifndef TRUECOLOR_HPP_
#define TRUECOLOR_HPP_

#include <cstdint>

namespace image
{

    struct TrueColor
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };

}

#endif
