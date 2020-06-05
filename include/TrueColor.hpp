/*
 * @file include/TrueColor.hpp
 * TrueColor.hpp
 *
 *  Created on: Nov 18, 2018
 *      Author: Antero Nousiainen
 */

#ifndef TRUECOLOR_HPP_
#define TRUECOLOR_HPP_

#include <cstdint>
#include <tuple>
#include <fmt/format.h>

namespace image
{

    /**
        A 24bit 8:8:8 color
    */
    struct TrueColor
    {
        using Sample = uint8_t;

        bool operator==(const TrueColor & other) const
        {
            return std::tie(blue, green, red) == std::tie(other.blue, other.green, other.red);
        }

        constexpr inline static std::uint16_t bits = 24;

        Sample blue;
        Sample green;
        Sample red;
    };

}

namespace fmt
{

    /**
        @brief  <a href="https://fmt.dev/latest/api.html#formatting-user-defined-types">
                Formatter</a> for image::TrueColor
    */
    template<>
    struct formatter<image::TrueColor>
    {
        constexpr auto parse(format_parse_context & ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const image::TrueColor & color, FormatContext & ctx)
        {
            return format_to(ctx.out(), "{{{:#04x}, {:#04x}, {:#04x}}}",
                color.blue, color.green, color.red);
        }
    };

}

#endif
