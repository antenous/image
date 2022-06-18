/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include "DirectX.hpp"

using namespace image;

namespace
{
    constexpr auto inchesInMeter = 39.3700787;

    constexpr std::uint32_t operator"" _dpi(unsigned long long int n)
    {
        return static_cast<std::uint32_t>(n*inchesInMeter);
    }

    std::uint32_t bytesInRow(std::int32_t width, std::uint16_t bits)
    {
        return ((bits*width + 31)/32*4);
    }

    std::uint32_t imageSize(std::int32_t height, std::int32_t width, std::uint16_t bits)
    {
        return bytesInRow(width, bits)*height;
    }

    Bitmap::Header::Info makeInfoHeader(std::int32_t height, std::int32_t width, std::uint16_t bits)
    {
        return {
            sizeof(Bitmap::Header::Info), width, height, 1, bits, 0,
            imageSize(height, width, bits), 72_dpi, 72_dpi, 0, 0 };
    }

    Bitmap::Header::File makeFileHeader(const Bitmap::Header::Info & infoHeader)
    {
        const std::uint32_t offset = sizeof(Bitmap::Magic) + sizeof(Bitmap::Header::File) + infoHeader.size;
        return { offset + infoHeader.imageSize, 0, offset };
    }
}

Bitmap::operator bool() const
{
    return magic == DirectX::detail::makeMagic<Magic>({'B', 'M'});
}

Bitmap Bitmap::make(std::int32_t height, std::int32_t width, const Data & data)
{
    Bitmap bmp;

    bmp.magic = DirectX::detail::makeMagic<Magic>({'B', 'M'});
    bmp.header.info = makeInfoHeader(height, width, Data::value_type::bits);
    bmp.header.file = makeFileHeader(bmp.header.info);
    bmp.data = data;

    return bmp;
}

int32_t Bitmap::height() const
{
    return header.info.height;
}

int32_t Bitmap::width() const
{
    return header.info.width;
}

uint8_t Bitmap::padding() const
{
    return header.info.padding();
}

uint8_t Bitmap::Header::Info::padding() const
{
    return static_cast<uint8_t>(bytesInRow(width, bits) - width*3);
}
