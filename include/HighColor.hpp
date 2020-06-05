/*
 * @file include/HighColor.hpp
 * HighColor.hpp
 *
 *  Created on: Jan 12, 2019
 *      Author: Antero Nousiainen
 */

#ifndef HIGHCOLOR_HPP_
#define HIGHCOLOR_HPP_

#include <cstdint>
#include <type_traits>
#include <fmt/format.h>

namespace image
{

    class HighColor
    {
    public:
        enum class Mask: uint16_t
        {
            Red   = 0xf800,
            Green = 0x7e0,
            Blue  = 0x1f
        };

        using Color = uint16_t;

        HighColor() = default;

        HighColor(Color color) noexcept;

        bool operator==(const HighColor & rhs) const noexcept;

        bool operator>(const HighColor & rhs) const noexcept;

        Color operator[](Mask mask) const noexcept;

    private:
        friend auto interpolate(const HighColor & lhs, const HighColor & rhs) noexcept;
        friend auto blend(const HighColor & lhs, const HighColor & rhs) noexcept;
        friend auto distance(const HighColor & lhs, const HighColor & rhs) noexcept;
        friend struct fmt::formatter<HighColor>;

        Color color{ 0 };
    };

    namespace detail
    {
        template<typename E>
        constexpr auto toUnderlying(E enumerator) noexcept
        {
            return static_cast<std::underlying_type_t<E>>(enumerator);
        }

        constexpr int DeBruijnBitPosition[32]{
            0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
            31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9 };

        /**
            Return the index of the least significant bit

            A de Bruijn sequence can be used to quickly find the index
            of the least significant set bit (i.e. the "right-most 1")
            https://en.wikipedia.org/wiki/De_Bruijn_sequence#Uses
        */
        constexpr auto indexOfLsb(HighColor::Mask mask) noexcept
        {
            const auto v(toUnderlying(mask));
            return DeBruijnBitPosition[static_cast<uint32_t>((v & -v) * 0x077CB531U) >> 27];
        }

        inline auto sample(HighColor::Mask mask, HighColor::Color color) noexcept
        {
            return color & toUnderlying(mask);
        }

        inline auto interpolate(HighColor::Mask mask, HighColor::Color lhs, HighColor::Color rhs) noexcept
        {
            return sample(mask, (2*sample(mask, lhs) + sample(mask, rhs))/3);
        }

        inline auto blend(HighColor::Mask mask, HighColor::Color lhs, HighColor::Color rhs) noexcept
        {
            return sample(mask, (sample(mask, lhs) + sample(mask, rhs))/2);
        }

        inline auto distance(HighColor::Mask mask, HighColor::Color lhs, HighColor::Color rhs) noexcept
        {
            const auto delta((sample(mask, lhs) - sample(mask, rhs)) >> indexOfLsb(mask));
            return delta*delta;
        }
    }

    inline HighColor::HighColor(HighColor::Color color) noexcept:
        color(color)
    {}

    inline bool HighColor::operator==(const HighColor & rhs) const noexcept
    {
        return color == rhs.color;
    }

    inline bool HighColor::operator>(const HighColor & rhs) const noexcept
    {
        return color > rhs.color;
    }

    inline HighColor::Color HighColor::operator[](Mask mask) const noexcept
    {
        return detail::sample(mask, color) >> detail::indexOfLsb(mask);
    }

    /**
        Calculate intermediate colors with linear
        [interpolation](https://docs.microsoft.com/fi-fi/windows/desktop/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc1)

        When color_0 > color_1 (no alpha) the intermediate colors
        (color_2 and color_3) are calculated with linear interpolation:
        > color_2 = 2/3*color_0 + 1/3*color_1\n
        > color_3 = 1/3*color_0 + 2/3*color_1
    */
    inline auto interpolate(const HighColor & lhs, const HighColor & rhs) noexcept
    {
        return HighColor(
            detail::interpolate(HighColor::Mask::Red, lhs.color, rhs.color) |
            detail::interpolate(HighColor::Mask::Green, lhs.color, rhs.color) |
            detail::interpolate(HighColor::Mask::Blue, lhs.color, rhs.color));
    }

    /**
        Calculate intermediate colors with linear
        [blend](https://docs.microsoft.com/fi-fi/windows/desktop/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc1)

        When color_1 > color_0 (1-bit alpha) the color_3 is set to 0
        and color_2 is a linear blend of color_0 and color_1:
        > color_2 = 1/2*color_0 + 1/2*color_1\n
        > color_3 = 0
    */
    inline auto blend(const HighColor & lhs, const HighColor & rhs) noexcept
    {
        return HighColor(
            detail::blend(HighColor::Mask::Red, lhs.color, rhs.color) |
            detail::blend(HighColor::Mask::Green, lhs.color, rhs.color) |
            detail::blend(HighColor::Mask::Blue, lhs.color, rhs.color));
    }

    /**
        Return the distance between two colors

        The standard means of determining distances is the
        [Euclidean](https://en.wikipedia.org/wiki/Color_difference#Euclidean) distance
    */
    inline auto distance(const HighColor & lhs, const HighColor & rhs) noexcept
    {
        return
            detail::distance(HighColor::Mask::Red, lhs.color, rhs.color) +
            detail::distance(HighColor::Mask::Green, lhs.color, rhs.color) +
            detail::distance(HighColor::Mask::Blue, lhs.color, rhs.color);
    }

}

namespace fmt
{

    /**
        @brief  <a href="https://fmt.dev/latest/api.html#formatting-user-defined-types">
                Formatter</a> for image::HighColor
    */
    template<>
    struct formatter<image::HighColor> : formatter<unsigned short>
    {
        template<typename FormatContext>
        auto format(const image::HighColor & color, FormatContext & ctx)
        {
            return formatter<unsigned short>::format(color.color, ctx);
        }
    };

}

#endif
