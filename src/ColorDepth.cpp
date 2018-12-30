/*
 * ColorDepth.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorDepth.hpp"
#include <bitset>

using namespace image;

namespace
{
    inline TrueColor::Sample upscaleSample(HighColor::Sample sample, uint8_t bits) noexcept
    {
        sample <<= (8 - bits);
        return sample | (sample >> bits);
    }

    inline HighColor::Sample upscaleSample(HighColor::Mask mask, const HighColor & color) noexcept
    {
        return upscaleSample(color[mask], std::bitset<16>(static_cast<std::underlying_type_t<HighColor::Mask>>(mask)).count());
    }
}

HighColor ColorDepth::trueToHigh(const TrueColor & trueColor) noexcept
{
    return HighColor(((trueColor.red & 0xf8) << 8)|((trueColor.green & 0xfc) << 3)|((trueColor.blue & 0xf8) >> 3));
}

TrueColor ColorDepth::highToTrue(const HighColor & highColor) noexcept
{
    return {
        .blue  = upscaleSample(HighColor::Mask::Blue, highColor),
        .green = upscaleSample(HighColor::Mask::Green, highColor),
        .red   = upscaleSample(HighColor::Mask::Red, highColor) };
}
