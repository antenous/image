/*
 * BlockCompressor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#include "BlockCompressor.hpp"
#include <algorithm>
#include "ColorDepth.hpp"
#include "ColorPalette.hpp"

using namespace image;

BlockCompressor::BadSize::BadSize() :
    std::invalid_argument("bad size")
{}

namespace
{
    using Color = std::array<HighColor, 4>;
    using IntermediateColors = std::pair<HighColor, HighColor>;

    IntermediateColors interpolate(const Color & color)
    {
        return { interpolate(color[0], color[1]), interpolate(color[1], color[0]) };
    }

    template<typename InputIterator>
    auto referenceColors(InputIterator first, InputIterator last)
    {
        DirectDrawSurface::Texel::ReferenceColors colors;

        for (int maxDist(-1); first != last; ++first)
            for (auto second(std::next(first)); second != last; ++second)
                if (auto dist(distance(*first, *second)); dist > maxDist)
                    std::tie(maxDist, colors.first, colors.second) = std::tie(dist, *first, *second);

        return colors;
    }

    auto removeAlpha(DirectDrawSurface::Texel::ReferenceColors && colors)
    {
        if (colors.second > colors.first)
            std::swap(colors.second, colors.first);

        return std::move(colors);
    }

    template<typename InputIterator>
    Color createColorTable(InputIterator first, InputIterator last)
    {
        Color color;
        std::tie(color[0], color[1]) = removeAlpha(referenceColors(first, last));
        std::tie(color[2], color[3]) = interpolate(color);
        return color;
    }

    DirectDrawSurface::Texel::ReferenceColors referenceColors(const Color & color)
    {
        return { color[0], color[1] };
    }

    uint8_t findNearest(const Color & color, HighColor ref)
    {
        const auto nearest([ref](auto x, auto y){ return distance(x, ref) < distance(y, ref); });
        return std::distance(color.begin(), std::min_element(color.begin(), color.end(), nearest));
    }

    template<typename InputIterator>
    uint32_t createLookupTable(const Color & color, InputIterator first, InputIterator last)
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
        const auto color(createColorTable(first, last));
        result = { referenceColors(color), createLookupTable(color, first, last) };
        return ++result;
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator compress(InputIterator first, InputIterator last, OutputIterator result)
    {
        for (; first != last; std::advance(first, 16))
            result = compressBlock(first, std::next(first, 16), result);

        return result;
    }
}

DirectDrawSurface::Data BlockCompressor::compress(
    const Bitmap::Colors & in, int32_t height, int32_t width)
{
    if (in.size() == 0 || in.size() % 16 != 0)
        throw BadSize();

    return compress(ColorPalette::rearrangeForDirectDrawSurface(
        height, width, ColorDepth::trueToHigh(in)));
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
    bool hasAlpha(const Color & color)
    {
        return color[1] > color[0];
    }

    IntermediateColors blend(const Color & color)
    {
        return { blend(color[0], color[1]), HighColor() };
    }

    Color recreateColorTable(const DirectDrawSurface::Texel::ReferenceColors & referenceColors)
    {
        Color color;
        std::tie(color[0], color[1]) = referenceColors;
        std::tie(color[2], color[3]) = hasAlpha(color) ? blend(color) : interpolate(color);
        return color;
    }

    template<typename OutputIterator>
    OutputIterator decompress(const DirectDrawSurface::Texel & texel, OutputIterator result)
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

    return ColorDepth::highToTrue(ColorPalette::rearrangeForBitmap(
        height, width, decompress(in)));
}

std::vector<HighColor> BlockCompressor::decompress(const DirectDrawSurface::Data & in)
{
    std::vector<HighColor> out;
    out.reserve(in.size()*16);
    ::decompress(in.begin(), in.end(), std::back_inserter(out));

    return out;
}
