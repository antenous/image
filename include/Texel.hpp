/*
 * @file include/Texel.hpp
 * Texel.hpp
 *
 *  Created on: Jun 9, 2019
 *      Author: Antero Nousiainen
 */

#ifndef TEXEL_HPP_
#define TEXEL_HPP_

#include <array>
#include <fmt/ranges.h>
#include "HighColor.hpp"

namespace image
{

    /**
        A 4x4 pixel color block with 2 16-bit reference colors and a 32-bit lookup table
    */
    class Texel
    {
    public:
        using ReferenceColors = std::array<HighColor, 2>;

        using LookupTable = std::uint32_t;

        ReferenceColors referenceColors{};

        LookupTable lookupTable{};

        bool operator==(const Texel & other) const;

        constexpr static std::uint8_t pixels()
        {
            return 4*4;
        }

        constexpr static std::uint8_t size()
        {
            return sizeof(Texel);
        }
    };

}

namespace fmt
{

    /**
        @brief  <a href="https://fmt.dev/latest/api.html#formatting-user-defined-types">
                Formatter</a> for image::Texel
     */
    template<>
    struct formatter<image::Texel>
    {
        constexpr auto parse(format_parse_context & ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const image::Texel & texel, FormatContext & ctx)
        {
            return format_to(ctx.out(), "{{{}, {:#b}}}",
                texel.referenceColors, texel.lookupTable);
        }
    };

}

#endif
