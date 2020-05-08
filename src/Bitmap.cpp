/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"

using namespace image;

namespace
{
    constexpr auto inchesInMeter = 39.3700787;

    constexpr std::uint32_t operator"" _dpi(unsigned long long int n)
    {
        return n*inchesInMeter;
    }

    std::uint32_t bytesInRow(std::int32_t width, std::uint16_t bits)
    {
        return ((bits*width + 31)/32*4);
    }

    std::uint32_t imageSize(std::int32_t height, std::int32_t width, std::uint16_t bits)
    {
        return bytesInRow(width, bits)*height;
    }

    Bitmap::InfoHeader makeInfoHeader(std::int32_t height, std::int32_t width, std::uint16_t bits)
    {
        return {
            sizeof(Bitmap::InfoHeader), width, height, 1, bits, 0,
            imageSize(height, width, bits), 72_dpi, 72_dpi, 0, 0 };
    }

    Bitmap::FileHeader makeFileHeader(const Bitmap::InfoHeader & infoHeader)
    {
        return {{ 'B', 'M' }, 14 + infoHeader.size + infoHeader.imageSize, 0, 14 + infoHeader.size };
    }
}

Bitmap::operator bool() const
{
    return fileHeader.type[0] == 'B' && fileHeader.type[1] == 'M';
}

Bitmap Bitmap::make(std::int32_t height, std::int32_t width, const Colors & colors)
{
    Bitmap bmp;

    bmp.infoHeader = makeInfoHeader(height, width, Colors::value_type::bits);
    bmp.fileHeader = makeFileHeader(bmp.infoHeader);
    bmp.colors = colors;

    return bmp;
}

int32_t Bitmap::height() const
{
    return infoHeader.height;
}

int32_t Bitmap::width() const
{
    return infoHeader.width;
}

uint8_t Bitmap::padding() const
{
    return infoHeader.padding();
}

uint8_t Bitmap::InfoHeader::padding() const
{
    return bytesInRow(width, bits) - width*3;
}
