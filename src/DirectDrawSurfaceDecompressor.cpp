/*
 * DirectDrawSurfaceDecompressor.cpp
 *
 *  Created on: Jun 23, 2019
 *      Author: Antero Nousiainen
 */

#include "DirectDrawSurfaceDecompressor.hpp"
#include "BlockCodec.hpp"
#include "BlockIterator.hpp"
#include "ColorDepth.hpp"
#include "TransformIterator.hpp"

using namespace image;

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

DirectDrawSurfaceDecompressor::BadSize::BadSize() :
    std::invalid_argument("bad size")
{}

Bitmap::Colors DirectDrawSurfaceDecompressor::decompress(
    const DirectDrawSurface::Data & in, std::int32_t height, std::int32_t width)
{
    if (in.empty())
        throw BadSize();

    Bitmap::Colors out(in.size()*Texel::pixels());
    ::decompress(in.begin(), in.end(), deblocker(out, height, width) | transformed(ColorDepth::highToTrue));
    return out;
}
