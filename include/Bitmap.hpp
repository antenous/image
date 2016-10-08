/*
 * @file include/Bitmap.hpp
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

    class DirectDrawSurface;

    /**
        @brief  Class representation of a bitmap image file

        This class is used to load a bitmap image from a file or to convert a
        direct draw surface image into a bitmap. Once the image has been loaded
        into the class it can be saved to a file.
    */
    class Bitmap
    {
    public:
        class BadFile;

        class InvalidType;

        class BadDirectDrawSurface;

        typedef std::vector< uint32_t > Palette;

        virtual ~Bitmap() = default;

        /**
            Check if the image has been loaded or converted successfully

            @return true when bitmap has been successfully loaded
        */
        virtual explicit operator bool() const;

        /**
            Load a bitmap image from a file

            @param file File to load the image from

            @throw BadFile if unable to load from the file
            @throw InvalidType if file is not a bitmap image
        */
        void loadFrom( std::istream & file );

        /**
            Convert a bitmap image from a direct draw surface image

            @param dds Direct draw surface image to convert from

            @throw BadDirectDrawSurface if the direct draw surface has not been loaded
        */
        void convertFrom( const DirectDrawSurface & dds );

        /**
            Save the loaded image to a file

            @param file File to load to

            @throw BadFile if unable to save to the file
            @throw InvalidType if bitmap has not been loaded
        */
        void saveTo( std::ostream & file ) const;

        /**
            Return height of the image

            @return Height of the image
        */
        virtual int32_t getHeight() const;

        /**
            Return width of the image

            @return Width of the image
        */
        virtual int32_t getWidth() const;

        /**
            Return the image color palette

            @return Image color palette
        */
        virtual Palette getPalette() const;

    private:
        void readFileHeader( std::istream & file );

        void readInfoHeader( std::istream & file );

        void readPalette( std::istream & file );

        void createInfoHeader( const DirectDrawSurface & dds );

        void createFileHeader();

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
        } fileHeader{{ 0, 0 }, 0, 0, 0, 0 };

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

    class Bitmap::BadDirectDrawSurface : public std::runtime_error
    {
    public:
        BadDirectDrawSurface() :
            std::runtime_error( "bad direct draw surface" )
        {}
    };

}

#endif
