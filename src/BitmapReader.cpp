/*
 * BitmapReader.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapReader.hpp"

using namespace image;

namespace
{
    template<typename T>
    void read(std::istream & stream, T & t, std::streamsize count = sizeof(T))
    {
        stream.read(reinterpret_cast<char*>(&t), count);
    }

    inline void skipPadding(std::istream & from, uint8_t padding)
    {
        from.seekg(padding, from.cur);
    }

    void read(std::istream & from, Bitmap::Data & data, int32_t height, int32_t width, uint8_t padding)
    {
        data.resize(height*width);

        for (int32_t row(0), firstInRow(0); row < height; ++row, firstInRow += width, skipPadding(from, padding))
            read(from, data[firstInRow], width*sizeof(Bitmap::Data::value_type));
    }

    Bitmap readBitmap(std::istream & from)
    {
        Bitmap bmp;
        read(from, bmp.magic);

        if (!bmp)
            throw BitmapReader::InvalidType();

        read(from, bmp.header.file);
        read(from, bmp.header.info);
        read(from, bmp.data, bmp.height(), bmp.width(), bmp.padding());

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
