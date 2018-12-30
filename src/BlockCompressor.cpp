/*
 * BlockCompressor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#include "BlockCompressor.hpp"
#include <algorithm>
#include <array>
#include <tuple>
#include "ColorDepth.hpp"
#include "ColorPalette.hpp"

using namespace image;

BlockCompressor::BadSize::BadSize() :
    std::invalid_argument("bad size")
{}

namespace
{
    using Color = std::array<uint16_t, 4>;

    enum Mask: uint16_t
    {
        red   = 0xf800,
        green = 0x7e0,
        blue  = 0x1f
    };

    uint16_t interpolate(Mask mask, uint16_t c0, uint16_t c1)
    {
        return (2*(c0 & mask) + (c1 & mask))/3 & mask;
    }

    uint16_t interpolate(uint16_t c0, uint16_t c1)
    {
        return interpolate(red, c0, c1) | interpolate(green, c0, c1) | interpolate(blue, c0, c1);
    }

    std::pair<uint16_t, uint16_t> interpolate(const Color & color)
    {
        return { interpolate(color[0], color[1]), interpolate(color[1], color[0]) };
    }

    auto distance(uint16_t c0, uint16_t c1)
    {
        const auto r(((c0 & red) - (c1 & red)) >> 11);
        const auto g(((c0 & green) - (c1 & green)) >> 5);
        const auto b((c0 & blue) - (c1 & blue));
        return r*r + g*g + b*b;
    }

    template<typename InputIterator>
    std::pair<uint16_t, uint16_t> referenceColors(InputIterator first, InputIterator last)
    {
        std::pair<uint16_t, uint16_t> colors;
        int maxDist(-1);

        for (; first != last; ++first)
            for (auto second(std::next(first)); second != last; ++second)
                if (auto dist(distance(*first, *second)); dist > maxDist)
                    std::tie(maxDist, colors.first, colors.second) = std::tie(dist, *first, *second);

        return colors;
    }

    std::pair<uint16_t, uint16_t> reorder(std::pair<uint16_t, uint16_t> && colors)
    {
        if (colors.second > colors.first)
            return { colors.second, colors.first };

        return std::move(colors);
    }

    template<typename InputIterator>
    Color createColorTable(InputIterator first, InputIterator last)
    {
        Color color;
        std::tie(color[0], color[1]) = reorder(referenceColors(first, last));
        std::tie(color[2], color[3]) = interpolate(color);
        return color;
    }

    DirectDrawSurface::Texel::ReferenceColors referenceColors(const Color & color)
    {
        return { color[0], color[1] };
    }

    uint8_t findNearest(const Color & color, uint16_t ref)
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

DirectDrawSurface::Data BlockCompressor::compress(const std::vector<uint16_t> & in)
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
        return color[0] <= color[1];
    }

    uint16_t blend(Mask mask, uint16_t a, uint16_t b)
    {
        return ((a & mask) + (b & mask))/2 & mask;
    }

    uint16_t blend(uint16_t a, uint16_t b)
    {
        return blend(red, a, b) | blend(green, a, b) | blend(blue, a, b);
    }

    std::pair<uint16_t, uint16_t> blend(const Color & color)
    {
        return { blend(color[0], color[1]), 0xffff };
    }

    Color recreateColorTable(const DirectDrawSurface::Texel::ReferenceColors & referenceColors)
    {
        Color color;
        std::tie(color[0], color[1]) = std::tie(referenceColors[0], referenceColors[1]);
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

std::vector<uint16_t> BlockCompressor::decompress(const DirectDrawSurface::Data & in)
{
    std::vector<uint16_t> out;
    out.reserve(in.size()*16);
    ::decompress(in.begin(), in.end(), std::back_inserter(out));

    return out;
}
