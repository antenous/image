/*
 * DirectX.hpp
 *
 *  Created on: Apr 20, 2019
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTX_HPP_
#define DIRECTX_HPP_

#include <array>
#include <cstdint>

/**
    A subset of DirectX constants originally defined in
    [dds.h](https://github.com/Microsoft/DirectXTex/blob/master/DirectXTex/DDS.h)
*/

namespace DirectX
{

    namespace detail
    {
        template<typename Magic>
        constexpr auto makeMagic(std::array<std::uint8_t, sizeof(Magic)> ch)
        {
            Magic magic = 0;

            for (auto first = std::rbegin(ch), last = std::rend(ch); first != last; ++first)
                magic = (magic << 8) | *first;

            return magic;
        }
    }

    constexpr std::uint32_t DDS_MAGIC{detail::makeMagic<std::uint32_t>({'D', 'D', 'S', ' '})};

    constexpr std::uint32_t DDS_HEADER_FLAGS_PITCH{     0x8};
    constexpr std::uint32_t DDS_HEADER_FLAGS_TEXTURE{   0x1007};
    constexpr std::uint32_t DDS_HEADER_FLAGS_MIPMAP{    0x20000};
    constexpr std::uint32_t DDS_HEADER_FLAGS_LINEARSIZE{0x80000};
    constexpr std::uint32_t DDS_HEADER_FLAGS_VOLUME{    0x800000};

    constexpr std::uint32_t DDS_SURFACE_FLAGS_CUBEMAP{0x8};
    constexpr std::uint32_t DDS_SURFACE_FLAGS_TEXTURE{0x1000};
    constexpr std::uint32_t DDS_SURFACE_FLAGS_MIPMAP{ 0x400008};

    constexpr std::uint32_t DDS_FOURCC{0x4};
    constexpr std::uint32_t DDS_DXT1{detail::makeMagic<std::uint32_t>({'D', 'X', 'T', '1'})};

}

#endif
