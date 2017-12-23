/*
 * BitmapReader.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapReader.hpp"
#include <numeric>
#include "BitmapDataReader.hpp"
#include "Reader.hpp"

using namespace image;

namespace
{
    void read(std::istream & from, decltype(Bitmap::fileHeader) & fileHeader)
    {
        Reader::read(from, std::tie(
            fileHeader.type[0],
            fileHeader.type[1],
            fileHeader.size,
            fileHeader.reserved,
            fileHeader.offset));
    }

    void read(std::istream & from, decltype(Bitmap::infoHeader) & infoHeader)
    {
        Reader::read(from, std::tie(
            infoHeader.size,
            infoHeader.width,
            infoHeader.height,
            infoHeader.planes,
            infoHeader.bits,
            infoHeader.compression,
            infoHeader.imageSize,
            infoHeader.horizontalResolution,
            infoHeader.verticalResolution,
            infoHeader.colors,
            infoHeader.importantColors));
    }

    void read(std::istream & file, decltype(Bitmap::colors) & colors, int32_t width, int32_t height)
    {
        colors.resize(height*width);
        auto it(colors.begin());

        for (const auto & line : BitmapDataReader(file, width, height))
            for (auto first(line.begin()), last(line.end()); first != last; std::advance(first, 3), ++it)
                *it = std::accumulate(first, std::next(first, 3), 0, [](uint32_t i, uint8_t j){ return i << 8 | j; });
    }

    Bitmap readBitmap(std::istream & from)
    {
        Bitmap bmp;

        read(from, bmp.fileHeader);

        if (!bmp)
            throw BitmapReader::InvalidType();

        read(from, bmp.infoHeader);
        read(from, bmp.colors, bmp.infoHeader.width, bmp.infoHeader.height);

        return bmp;
    }
}

BitmapReader::BadFile::BadFile() :
    std::invalid_argument("bad file")
{}

BitmapReader::InvalidType::InvalidType() :
    std::runtime_error("invalid type")
{}

Bitmap BitmapReader::read(std::istream && from)
{
    if (!from)
        throw BitmapReader::BadFile();

    from.exceptions(std::istream::failbit | std::istream::badbit);

    try
    {
        return readBitmap(from);
    }
    catch (const std::istream::failure &)
    {
        throw BadFile();
    }
}
