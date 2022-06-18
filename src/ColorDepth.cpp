/*
 * ColorDepth.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorDepth.hpp"

using namespace image;

namespace
{
    constexpr std::size_t bitsInSample = 8;

    constexpr std::size_t bitCount(std::uint16_t i) noexcept
    {
        return (i == 0) ? 0 : 1 + bitCount(i & (i - 1));
    }

    constexpr auto bitCount(HighColor::Mask mask) noexcept
    {
        return bitCount(detail::toUnderlying(mask));
    }

    inline HighColor::Color downscale(TrueColor::Sample sample, HighColor::Mask mask) noexcept
    {
        sample >>= (bitsInSample - bitCount(mask));
        return static_cast<HighColor::Color>(sample) << detail::indexOfLsb(mask);
    }

    inline TrueColor::Sample upscale(HighColor::Color sample, std::size_t bits) noexcept
    {
        sample <<= (bitsInSample - bits);
        return static_cast<TrueColor::Sample>(sample | (sample >> bits));
    }
}

HighColor ColorDepth::trueToHigh(const TrueColor & trueColor) noexcept
{
    return HighColor(
        downscale(trueColor.red,   HighColor::Mask::Red)  |
        downscale(trueColor.green, HighColor::Mask::Green)|
        downscale(trueColor.blue,  HighColor::Mask::Blue));
}

TrueColor ColorDepth::highToTrue(const HighColor & highColor) noexcept
{
    return {
        upscale(highColor[HighColor::Mask::Blue],  bitCount(HighColor::Mask::Blue)),
        upscale(highColor[HighColor::Mask::Green], bitCount(HighColor::Mask::Green)),
        upscale(highColor[HighColor::Mask::Red],   bitCount(HighColor::Mask::Red)) };
}
