/*
 * Bitmap.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#ifndef BITMAP_HPP_
#define BITMAP_HPP_

#include <stdexcept>
#include <vector>

namespace image
{

    class Bitmap
    {
    public:
        class BadFile;

        class InvalidType;

        typedef std::vector< uint32_t > Palette;

        virtual ~Bitmap() = default;

        void loadFrom( std::istream & file );

        void saveTo( std::ostream & file ) const;

        virtual int32_t getHeight() const;

        virtual int32_t getWidth() const;

        virtual Palette getPalette() const;

    private:
        void readFileHeader( std::istream & file );

        void readInfoHeader( std::istream & file );

        void readPalette( std::istream & file );

        void writeFileHeader( std::ostream & file ) const;

        void writeInfoHeader( std::ostream & file ) const;

        void writePalette( std::ostream & file ) const;

        void writePaletteRow( std::ostream & file, typename Palette::size_type row ) const;

        struct
        {
            char type[2];
            uint32_t size;
            uint16_t reserved1;
            uint16_t reserved2;
            uint32_t offset;
        } fileHeader;

        struct
        {
            uint32_t size;
            int32_t width;
            int32_t height;
            uint16_t planes;
            uint16_t bits;
            uint32_t compression;
            uint32_t imageSize;
            uint32_t horizontalResolution;
            uint32_t verticalResolution;
            uint32_t colors;
            uint32_t importantColors;
        } infoHeader;

        uint8_t padding;

        Palette palette;
    };

    class Bitmap::BadFile : public std::runtime_error
    {
    public:
        BadFile() :
            std::runtime_error( "bad file" )
        {}
    };

    class Bitmap::InvalidType : public std::runtime_error
    {
    public:
        InvalidType() :
            std::runtime_error( "invalid type" )
        {}
    };

}

#endif
