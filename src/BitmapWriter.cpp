/*
 * BitmapWriter.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapWriter.hpp"
#include <algorithm>
#include "Writer.hpp"

using namespace image;

namespace
{
    template<template<typename, typename> class Range, typename Alloc>
    class SampleCopyIterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
    {
    public:
        explicit SampleCopyIterator(Range<uint8_t, Alloc> & range) :
            it(range.begin())
        {}

        auto & operator=(uint32_t value)
        {
            for (auto shift : { 16, 8, 0 })
                *it++ = ((value >> shift) & 0xff);
            return *this;
        }

        auto & operator*(){ return *this; }

        auto & operator++(){ return *this; }

    private:
        typename Range<uint8_t, Alloc>::iterator it;
    };

    template<template<typename, typename> class Range, typename Alloc>
    auto sampleCopyer(Range<uint8_t, Alloc> & range)
    {
        return SampleCopyIterator<Range, Alloc>(range);
    }

    void write(std::ostream & to, const decltype(Bitmap::fileHeader) & fileHeader)
    {
        Writer::write(to, std::tie(
            fileHeader.type[0],
            fileHeader.type[1],
            fileHeader.size,
            fileHeader.reserved,
            fileHeader.offset));
    }

    void write(std::ostream & to, const decltype(Bitmap::infoHeader) & infoHeader)
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

    void write(std::ostream & to, const std::vector<uint8_t> & line)
    {
        to.write(reinterpret_cast<const char*>(line.data()), line.size());
    }

    const std::vector<uint8_t> & line(
        const decltype(Bitmap::colors)::const_iterator first,
        const decltype(Bitmap::colors)::const_iterator last, std::vector<uint8_t> & buffer)
    {
        std::copy(first, last, sampleCopyer(buffer));
        return buffer;
    }

    void write(std::ostream & to, const decltype(Bitmap::colors) & colors, int32_t width, uint8_t padding)
    {
        std::vector<uint8_t> buffer(width*3 + padding);

        for (auto first(colors.begin()), last(colors.end()); first != last; std::advance(first, width))
            write(to, line(first, std::next(first, width), buffer));
    }

    void writeBitmap(std::ostream & to, const Bitmap & bmp)
    {
        write(to, bmp.fileHeader);
        write(to, bmp.infoHeader);
        write(to, bmp.colors, bmp.infoHeader.width, bmp.padding());
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
