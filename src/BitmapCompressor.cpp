/*
 * BitmapCompressor.cpp
 *
 *  Created on: Jun 23, 2019
 *      Author: Antero Nousiainen
 */

#include "BitmapCompressor.hpp"
#include <algorithm>
#include "BlockCodec.hpp"
#include "BlockIterator.hpp"
#include "ColorDepth.hpp"
#include "TransformIterator.hpp"

using namespace image;

BitmapCompressor::BadSize::BadSize() :
    std::invalid_argument("bad size")
{}

namespace
{
    template<typename InputIterator, typename OutputIterator>
    OutputIterator compress(InputIterator first, InputIterator last, OutputIterator result)
    {
        for (; first != last; std::advance(first, 16))
            *result++ = BlockCodec::compress(first, std::next(first, 16));

        return result;
    }

    auto compress(std::vector<HighColor> && in)
    {
        DirectDrawSurface::Data out;
        out.reserve(in.size()/16);
        ::compress(in.begin(), in.end(), std::back_inserter(out));

        return out;
    }

    auto downscaleAndReorder(const Bitmap::Colors & in, std::int32_t height, std::int32_t width)
    {
        std::vector<HighColor> out;
        out.reserve(in.size());
        std::copy(
            BlockIterator<Bitmap::Colors::const_iterator>(in.begin(), height, width),
            BlockIterator<Bitmap::Colors::const_iterator>(height, width),
            std::back_inserter(out) | transformed(ColorDepth::trueToHigh));
        return out;
    }
}

DirectDrawSurface::Data BitmapCompressor::compress(
    const Bitmap::Colors & in, std::int32_t height, std::int32_t width)
{
    if (in.empty() || in.size() % 16 != 0)
        throw BadSize();

    return ::compress(downscaleAndReorder(in, height, width));
}
