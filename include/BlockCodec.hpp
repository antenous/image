/*
 * @file include/BlockCodec.hpp
 * BlockCodec.hpp
 *
 *  Created on: Jun 9, 2019
 *      Author: Antero Nousiainen
 */

#ifndef BLOCKCODEC_HPP_
#define BLOCKCODEC_HPP_

#include <algorithm>
#include <tuple>
#include "Texel.hpp"

namespace image
{

    /**
        @brief  A class to compress/decompress a 16-bit color block (a single texel)
    */
    class BlockCodec
    {
    public:
        template<typename InputIterator>
        static Texel compress(InputIterator first, InputIterator last);

        template<typename OutputIterator>
        static OutputIterator decompress(const Texel & texel, OutputIterator result);

    private:
        using ColorTable = std::array<HighColor, 4>;

        template<typename InputIterator>
        static auto referenceColors(InputIterator first, InputIterator last);

        static decltype(auto) removeAlpha(Texel::ReferenceColors && color);

        static bool hasAlpha(const Texel::ReferenceColors & color);

        static auto interpolate(const Texel::ReferenceColors & color);

        template<typename InputIterator>
        static auto createLookupTable(const ColorTable & color, InputIterator first, InputIterator last);

        static std::uint8_t findNearest(const ColorTable & color, HighColor ref);

        static auto recreateColorTable(const Texel::ReferenceColors & color);

        static auto blend(const Texel::ReferenceColors & color) -> ColorTable;
    };

    template<typename InputIterator>
    Texel BlockCodec::compress(InputIterator first, InputIterator last)
    {
        const auto color(removeAlpha(referenceColors(first, last)));
        return { color, createLookupTable(interpolate(color), first, last) };
    }

    template<typename InputIterator>
    auto BlockCodec::referenceColors(InputIterator first, InputIterator last)
    {
        Texel::ReferenceColors color;

        for (auto maxDist(-1); first != last; ++first)
            for (auto second(std::next(first)); second != last; ++second)
                if (auto dist(distance(*first, *second)); dist > maxDist)
                    std::tie(maxDist, color[0], color[1]) = std::tie(dist, *first, *second);

        return color;
    }

    inline decltype(auto) BlockCodec::removeAlpha(Texel::ReferenceColors && color)
    {
        if (hasAlpha(color))
            std::swap(color[1], color[0]);

        return std::move(color);
    }

    inline bool BlockCodec::hasAlpha(const Texel::ReferenceColors & color)
    {
        return color[1] > color[0];
    }

    inline auto BlockCodec::interpolate(const Texel::ReferenceColors & color)
    {
        return ColorTable{
            color[0], color[1],
            image::interpolate(color[0], color[1]), image::interpolate(color[1], color[0])};
    }

    template<typename InputIterator>
    auto BlockCodec::createLookupTable(const ColorTable & color, InputIterator first, InputIterator last)
    {
        Texel::LookupTable lookup(0);

        for (int y(24); first != last; y -= 8)
            for (unsigned x(0); x < 8; x += 2)
                lookup |= findNearest(color, *first++) << y << x;

        return lookup;
    }

    inline uint8_t BlockCodec::findNearest(const ColorTable & color, HighColor ref)
    {
        const auto nearest([ref](auto x, auto y){ return distance(x, ref) < distance(y, ref); });
        return std::distance(color.begin(), std::min_element(color.begin(), color.end(), nearest));
    }

    template<typename OutputIterator>
    OutputIterator BlockCodec::decompress(const Texel & texel, OutputIterator result)
    {
        const auto color(recreateColorTable(texel.referenceColors));

        for (int y(24); y >= 0; y -= 8)
            for (unsigned x(0); x < 8; x += 2)
                *result++ = color[((texel.lookupTable >> y >> x) & 0b11)];

        return result;
    }

    inline auto BlockCodec::recreateColorTable(const Texel::ReferenceColors & color)
    {
        return hasAlpha(color) ? blend(color) : interpolate(color);
    }

    inline auto BlockCodec::blend(const Texel::ReferenceColors & color) -> ColorTable
    {
        return {color[0], color[1], image::blend(color[0], color[1]), HighColor()};
    }

}

#endif
