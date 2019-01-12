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
    using SampleColor = uint8_t;

    HighColor trueToHigh(const TrueColor & trueColor)
    {
        return ((trueColor.red & 0xf8) << 8)|((trueColor.green & 0xfc) << 3)|((trueColor.blue & 0xf8) >> 3);
    }

    SampleColor upscaleSample(HighColor color, uint8_t bits)
    {
        const SampleColor sample((color << (8 - bits)) & 0xff);
        return sample | (sample >> bits);
    }

    TrueColor highToTrue(HighColor highColor)
    {
        TrueColor trueColor;

        trueColor.blue = upscaleSample(highColor, 5);
        highColor >>= 5;

        trueColor.green = upscaleSample(highColor, 6);
        highColor >>= 6;

        trueColor.red = upscaleSample(highColor, 5);
        return trueColor;
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

    std::transform(in.begin(), in.end(), std::back_inserter(out),[](HighColor color)
    {
        return ::highToTrue(color);
    });

    return out;
}
