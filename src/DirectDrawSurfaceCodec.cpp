/*
 * DirectDrawSurfaceCodec.cpp
 *
 *  Created on: May 22, 2020
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceCodec.hpp"
#include <algorithm>
#include "BlockCodec.hpp"
#include "BlockIterator.hpp"
#include "ColorDepth.hpp"
#include "TransformIterator.hpp"

using namespace image;

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

    auto downscaleAndReorder(std::int32_t height, std::int32_t width, const Bitmap::Data & in)
    {
        std::vector<HighColor> out;
        out.reserve(in.size());
        std::copy(
            BlockIterator<Bitmap::Data::const_iterator>(in.begin(), height, width),
            BlockIterator<Bitmap::Data::const_iterator>(height, width),
            std::back_inserter(out) | transformed(ColorDepth::trueToHigh));
        return out;
    }
}

DirectDrawSurfaceCodec::BadSize::BadSize() :
    std::invalid_argument("bad size")
{}

DirectDrawSurface::Data DirectDrawSurfaceCodec::compress(
    std::int32_t height, std::int32_t width, const Bitmap::Data & data)
{
    if (height % 4 != 0 || width % 4 != 0)
        throw BadSize();

    return ::compress(downscaleAndReorder(height, width, data));
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

Bitmap::Data DirectDrawSurfaceCodec::decompress(
    std::uint32_t height, std::uint32_t width, const DirectDrawSurface::Data & data)
{
    if (height % 4 != 0 || width % 4 != 0)
        throw BadSize();

    Bitmap::Data out(data.size()*Texel::pixels());
    ::decompress(data.begin(), data.end(), deblocker(out, height, width) | transformed(ColorDepth::highToTrue));
    return out;
}
