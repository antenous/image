/*
 * BitmapWriter.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapWriter.hpp"

using namespace image;

namespace
{
    template<typename T>
    void write(std::ostream & stream, const T & t, std::streamsize count = sizeof(T))
    {
        stream.write(reinterpret_cast<const char*>(&t), count);
    }

    void addPadding(std::ostream & to, uint8_t padding)
    {
        static const std::array<char, 3> bytes{};
        to.write(bytes.data(), padding);
    }

    void write(std::ostream & to, const Bitmap::Colors & colors, int32_t height, int32_t width, uint8_t padding)
    {
        for (int32_t row(0), firstInRow(0); row < height; ++row, firstInRow += width, addPadding(to, padding))
            write(to, colors[firstInRow], width*sizeof(Bitmap::Colors::value_type));
    }

    void writeBitmap(std::ostream & to, const Bitmap & bmp)
    {
        write(to, bmp.magic);
        write(to, bmp.fileHeader);
        write(to, bmp.infoHeader);
        write(to, bmp.colors, bmp.height(), bmp.width(), bmp.padding());
    }
}

BitmapWriter::BadFile::BadFile() :
    std::invalid_argument("bad file")
{}

BitmapWriter::InvalidType::InvalidType() :
    std::invalid_argument("invalid type")
{}

void BitmapWriter::write(std::ostream & to, const Bitmap & bmp)
{
    if (!to)
        throw BadFile();

    if (!bmp)
        throw InvalidType();

    writeBitmap(to, bmp);
}
