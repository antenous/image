/*
 * Bitmap.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <numeric>

using namespace image;

namespace
{
    inline void skipPadding(Bitmap::Data::const_iterator& it, uint8_t padding)
    {
        std::advance(it, padding);
    }

    inline uint32_t samplesToColor(Bitmap::Data::const_iterator it)
    {
        return std::accumulate(it, std::next(it, 3), 0, [](uint32_t i, uint8_t j)
        {
            return i << 8 | j;
        });
    }

    inline void addPadding(Bitmap::Data & data, uint8_t padding)
    {
        data.insert(data.end(), padding, 0);
    }

    inline uint8_t blue(uint32_t color)
    {
        return ((color >> 16) & 0xff);
    }

    inline uint8_t green(uint32_t color)
    {
        return ((color >> 8) & 0xff);
    }

    inline uint8_t red(uint32_t color)
    {
        return (color & 0xff);
    }
}

Bitmap::operator bool() const
{
    return fileHeader.type[0] == 'B' && fileHeader.type[1] == 'M';
}

int32_t Bitmap::getHeight() const
{
    return infoHeader.height;
}

int32_t Bitmap::getWidth() const
{
    return infoHeader.width;
}

Bitmap::Colors Bitmap::getColors() const
{
    Colors colors;
    colors.reserve(getHeight()*getWidth());

    for (auto it(data.begin()); it != data.end(); skipPadding(it, padding()))
        for (auto end(std::next(it, 3*getWidth())); it != end; std::advance(it, 3))
            colors.emplace_back(samplesToColor(it));

    return colors;
}

void Bitmap::setColors(const Colors & colors)
{
    Data data;
    data.reserve(infoHeader.imageSize);

    for (auto it(colors.begin()); it != colors.end(); addPadding(data, padding()))
        for (auto end(std::next(it, getWidth())); it != end; ++it)
            data.insert(data.end(), { blue(*it), green(*it), red(*it) });

    std::swap(data, this->data);
}

uint8_t Bitmap::padding() const
{
    const auto bytesInRow(( infoHeader.bits * infoHeader.width + 31 ) / 32 * 4 );
    return bytesInRow - infoHeader.width * 3;
}
