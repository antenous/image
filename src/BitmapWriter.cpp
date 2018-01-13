/*
 * BitmapWriter.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapWriter.hpp"
#include "Writer.hpp"

using namespace image;

namespace
{
    void write(std::ostream & to, const Bitmap::FileHeader & fileHeader)
    {
        Writer::write(to, std::tie(
            fileHeader.type[0],
            fileHeader.type[1],
            fileHeader.size,
            fileHeader.reserved,
            fileHeader.offset));
    }

    void write(std::ostream & to, const Bitmap::InfoHeader & infoHeader)
    {
        Writer::write(to, std::tie(
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

    void write(std::ostream & to, const Bitmap::Data & data)
    {
        to.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    void writeBitmap(std::ostream & to, const Bitmap & bmp)
    {
        write(to, bmp.fileHeader);
        write(to, bmp.infoHeader);
        write(to, bmp.data);
    }
}

BitmapWriter::BadFile::BadFile() :
    std::invalid_argument("bad file")
{}

BitmapWriter::InvalidType::InvalidType() :
    std::invalid_argument("invalid type")
{}

void BitmapWriter::write(std::ostream && to, const Bitmap & bmp)
{
    if (!to)
        throw BadFile();

    if (!bmp)
        throw InvalidType();

    writeBitmap(to, bmp);
}
