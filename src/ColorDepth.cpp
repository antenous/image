/*
 * ColorDepth.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorDepth.hpp"
#include <algorithm>
#include <initializer_list>

using namespace image;

namespace
{
    ColorDepth::SampleColor downscaleSample(ColorDepth::TrueColor color, uint8_t bits)
    {
        return (color & 0xff)>>(8 - bits);
    }

    ColorDepth::HighColor trueToHigh(ColorDepth::TrueColor trueColor)
    {
        ColorDepth::HighColor highColor(0);

        for (auto bitsToKeep : { 5, 6, 5 })
        {
            highColor <<= bitsToKeep;
            highColor |= downscaleSample(trueColor, bitsToKeep);
            trueColor >>= 8;
        }

        return highColor;
    }

    ColorDepth::SampleColor upscaleSample(ColorDepth::HighColor color, uint8_t bits)
    {
        const ColorDepth::SampleColor sample((color << (8 - bits)) & 0xff);
        return sample | (sample >> bits);
    }

    ColorDepth::TrueColor highToTrue(ColorDepth::HighColor highColor)
    {
        ColorDepth::TrueColor trueColor(0);

        for (auto bitsToKeep : { 5, 6, 5 })
        {
            trueColor <<= 8;
            trueColor |= upscaleSample(highColor, bitsToKeep);
            highColor >>= bitsToKeep;
        }

        return trueColor;
    }
}

std::vector<ColorDepth::HighColor> ColorDepth::trueToHigh(const std::vector<TrueColor> & in)
{
    std::vector<HighColor> out;
    out.reserve(in.size());

    std::transform(in.begin(), in.end(), std::back_inserter(out),[](TrueColor color)
    {
        return ::trueToHigh(color);
    });

    return out;
}

std::vector<ColorDepth::TrueColor> ColorDepth::highToTrue(const std::vector<HighColor> & in)
{
    std::vector<TrueColor> out;
    out.reserve(in.size());

    std::transform(in.begin(), in.end(), std::back_inserter(out),[](HighColor color)
    {
        return ::highToTrue(color);
    });

    return out;
}
