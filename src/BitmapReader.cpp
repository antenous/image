/*
 * BitmapReader.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapReader.hpp"
#include "Reader.hpp"

using namespace image;

namespace
{
    inline void skipPadding(std::istream & from, uint8_t padding)
    {
        from.seekg(padding, from.cur);
    }

    void read(std::istream & from, Bitmap::Colors & colors, int32_t height, int32_t width, uint8_t padding)
    {
        colors.resize(height*width);

        for (int32_t row(0), firstInRow(0); row < height; ++row, firstInRow += width, skipPadding(from, padding))
            from.read(reinterpret_cast<char*>(&colors[firstInRow]), width*sizeof(Bitmap::Colors::value_type));
    }

    Bitmap readBitmap(std::istream & from)
    {
        Bitmap bmp;
        Reader::read(from, bmp.magic);

        if (!bmp)
            throw BitmapReader::InvalidType();

        Reader::read(from, bmp.fileHeader);
        Reader::read(from, bmp.infoHeader);
        read(from, bmp.colors, bmp.height(), bmp.width(), bmp.padding());

        return bmp;
    }
}

BitmapReader::BadFile::BadFile() :
    std::invalid_argument("bad file")
{}

BitmapReader::InvalidType::InvalidType() :
    std::runtime_error("invalid type")
{}

Bitmap BitmapReader::read(std::istream & from)
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
