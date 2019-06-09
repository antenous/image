/*
 * BlockCompressor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#include "BlockCompressor.hpp"
#include <algorithm>
#include "BlockCodec.hpp"
#include "BlockIterator.hpp"
#include "ColorDepth.hpp"
#include "TransformIterator.hpp"

using namespace image;

BlockCompressor::BadSize::BadSize() :
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

    auto downscaleAndReorder(const Bitmap::Colors & in, int32_t height, int32_t width)
    {
        const auto downscaler([](const auto & it)
        {
            return TransformIterator(it, ColorDepth::trueToHigh);
        });

        std::vector<HighColor> out;
        out.reserve(in.size());
        std::copy(
            BlockIterator<Bitmap::Colors::const_iterator>(in.begin(), height, width),
            BlockIterator<Bitmap::Colors::const_iterator>(height, width),
            downscaler(std::back_inserter(out)));
        return out;
    }
}

DirectDrawSurface::Data BlockCompressor::compress(
    const Bitmap::Colors & in, int32_t height, int32_t width)
{
    if (in.size() == 0 || in.size() % 16 != 0)
        throw BadSize();

    return compress(downscaleAndReorder(in, height, width));
}

DirectDrawSurface::Data BlockCompressor::compress(const std::vector<HighColor> & in)
{
    DirectDrawSurface::Data out;
    out.reserve(in.size()/16);
    ::compress(in.begin(), in.end(), std::back_inserter(out));

    return out;
}

namespace
{
    template<typename InputIterator, typename OutputIterator>
    OutputIterator decompress(InputIterator first, InputIterator last, OutputIterator result)
    {
        for (; first != last; ++first)
            result = BlockCodec::decompress(*first, result);

        return result;
    }
}

Bitmap::Colors BlockCompressor::decompress(
    const DirectDrawSurface::Data & in, int32_t height, int32_t width)
{
    if (in.empty())
        throw BadSize();

    const auto upscaler([](const auto & it)
    {
        return TransformIterator(it, ColorDepth::highToTrue);
    });

    Bitmap::Colors out(in.size()*Texel::pixels());
    ::decompress(in.begin(), in.end(), upscaler(deblocker(out, height, width)));
    return out;
}
