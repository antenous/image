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
#include <ostream>
#include <tuple>

namespace image
{

    struct TrueColor
    {
        bool operator==(const TrueColor & other) const
        {
            return std::tie(blue, green, red) == std::tie(other.blue, other.green, other.red);
        }

        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };

    inline std::ostream & operator<<(std::ostream & os, const TrueColor & trueColor)
    {
        os << std::hex << "{"
            << "0x" << static_cast<int>(trueColor.blue) << ", "
            << "0x" << static_cast<int>(trueColor.green) << ", "
            << "0x" << static_cast<int>(trueColor.red) << "}";
        return os;
    }

}

#endif
