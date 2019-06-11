/*
 * BlockCompressor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#include "BlockCompressor.hpp"
#include <algorithm>
#include "BlockIterator.hpp"
#include "ColorDepth.hpp"
#include "TransformIterator.hpp"

using namespace image;

BlockCompressor::BadSize::BadSize() :
    std::invalid_argument("bad size")
{}

namespace
{
    using ColorTable = std::array<HighColor, 4>;

    ColorTable interpolate(const Texel::ReferenceColors & color)
    {
        return { color[0], color[1], interpolate(color[0], color[1]), interpolate(color[1], color[0]) };
    }

    template<typename InputIterator>
    auto referenceColors(InputIterator first, InputIterator last)
    {
        Texel::ReferenceColors colors;

        for (int maxDist(-1); first != last; ++first)
            for (auto second(std::next(first)); second != last; ++second)
                if (auto dist(distance(*first, *second)); dist > maxDist)
                    std::tie(maxDist, colors[0], colors[1]) = std::tie(dist, *first, *second);

        return colors;
    }

    bool hasAlpha(const Texel::ReferenceColors & color)
    {
        return color[1] > color[0];
    }

    auto removeAlpha(Texel::ReferenceColors && color)
    {
        if (hasAlpha(color))
            std::swap(color[1], color[0]);

        return std::move(color);
    }

    auto createColorTable(const Texel::ReferenceColors & color)
    {
        return interpolate(color);
    }

    uint8_t findNearest(const ColorTable & color, HighColor ref)
    {
        const auto nearest([ref](auto x, auto y){ return distance(x, ref) < distance(y, ref); });
        return std::distance(color.begin(), std::min_element(color.begin(), color.end(), nearest));
    }

    template<typename InputIterator>
    uint32_t createLookupTable(const ColorTable & color, InputIterator first, InputIterator last)
    {
        uint32_t lookup(0);

        for (int y(24); first != last; y -= 8)
            for (unsigned x(0); x < 8; x += 2)
                lookup |= findNearest(color, *first++) << y << x;

        return lookup;
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator compressBlock(InputIterator first, InputIterator last, OutputIterator result)
    {
        const auto color(removeAlpha(referenceColors(first, last)));
        result = { color, createLookupTable(createColorTable(color), first, last) };
        return ++result;
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator compress(InputIterator first, InputIterator last, OutputIterator result)
    {
        for (; first != last; std::advance(first, 16))
            result = compressBlock(first, std::next(first, 16), result);

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
    ColorTable blend(const Texel::ReferenceColors & color)
    {
        return { color[0], color[1], blend(color[0], color[1]), HighColor() };
    }

    ColorTable recreateColorTable(const Texel::ReferenceColors & color)
    {
        return hasAlpha(color) ? blend(color) : interpolate(color);
    }

    template<typename OutputIterator>
    OutputIterator decompress(const Texel & texel, OutputIterator result)
    {
        const auto color(recreateColorTable(texel.referenceColors));

        for (int y(24); y >= 0; y -= 8)
            for (unsigned x(0); x < 8; x += 2)
                *result++ = color[((texel.lookupTable >> y >> x) & 0b11)];

        return result;
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator decompress(InputIterator first, InputIterator last, OutputIterator result)
    {
        for (; first != last; ++first)
            result = decompress(*first, result);

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
