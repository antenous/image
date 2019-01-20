/*
 * ColorDepth.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorDepth.hpp"
#include <algorithm>
#include <bitset>

using namespace image;

namespace
{
    inline auto trueToHigh(const TrueColor & trueColor) noexcept
    {
        return HighColor(((trueColor.red & 0xf8) << 8)|((trueColor.green & 0xfc) << 3)|((trueColor.blue & 0xf8) >> 3));
    }

    inline TrueColor::Sample upscaleSample(HighColor::Sample sample, uint8_t bits) noexcept
    {
        sample <<= (8 - bits);
        return sample | (sample >> bits);
    }

    inline HighColor::Sample upscaleSample(HighColor::Mask mask, const HighColor & color) noexcept
    {
        return upscaleSample(color[mask], std::bitset<16>(static_cast<std::underlying_type_t<HighColor::Mask>>(mask)).count());
    }

    inline auto highToTrue(const HighColor & highColor) noexcept
    {
        return TrueColor{
            .blue  = upscaleSample(HighColor::Mask::Blue, highColor),
            .green = upscaleSample(HighColor::Mask::Green, highColor),
            .red   = upscaleSample(HighColor::Mask::Red, highColor) };
    }
}

std::vector<HighColor> ColorDepth::trueToHigh(const std::vector<TrueColor> & in)
{
    std::vector<HighColor> out;
    out.reserve(in.size());

    std::transform(in.begin(), in.end(), std::back_inserter(out),[](const TrueColor & color)
    {
        return ::trueToHigh(color);
    });

    return out;
}

std::vector<TrueColor> ColorDepth::highToTrue(const std::vector<HighColor> & in)
{
    std::vector<TrueColor> out;
    out.reserve(in.size());

    std::transform(in.begin(), in.end(), std::back_inserter(out),[](const HighColor & color)
    {
        return ::highToTrue(color);
    });

    return out;
}
