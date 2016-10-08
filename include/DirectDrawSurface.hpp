/*
 * @file include/DirectDrawSurface.hpp
 * DirectDrawSurface.hpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTDRAWSURFACE_HPP_
#define DIRECTDRAWSURFACE_HPP_

#include <stdexcept>
#include <vector>

namespace image
{

    class Bitmap;

    /**
        @brief  Class representation of a direct draw surface image

        This class is used to load a direct draw surface image from a file or
        to convert a bitmap image into a direct draw surface. Once the image
        has been loaded into the class it can be saved to a file.
    */
    class DirectDrawSurface
    {
    public:
        class BadFile;

        class InvalidType;

        class BadBitmap;

        typedef std::vector< uint32_t > Surface;

        virtual ~DirectDrawSurface() = default;

        /**
            Check if the image has been loaded or converted successfully

            @return true when direct draw surface has been successfully loaded
        */
        virtual explicit operator bool() const;

        /**
            Load a direct draw surface image from a file

            @param file File to load the image from

            @throw BadFile if unable to load from the file
            @throw InvalidType if file is not a direct draw surface image
        */
        void loadFrom( std::istream & file );

        /**
            Convert a direct draw surface image from a bitmap image

            @param bitmap Bitmap image to convert from

            @throw BadBitmap if the bitmap has not been loaded
        */
        void convertFrom( const Bitmap & bitmap );

        /**
            Save the loaded image to a file

            @param file File to load to

            @throw BadFile if unable to save to the file
            @throw InvalidType if the direct draw surface has not been loaded
        */
        void saveTo( std::ostream & file ) const;

        /**
            Return height of the image

            @return Height of the image
        */
        virtual uint32_t getHeight() const;

        /**
             Return width of the image

             @return Width of the image
        */
        virtual uint32_t getWidth() const;

        /**
            Return the image durface data

            @return Surface date
        */
        virtual Surface getSurface() const;

    private:
        void readMagic( std::istream & file );

        void readHeader( std::istream & file );

        void readSurfaceData( std::istream & file );

        uint32_t countSurfaceBlocks() const;

        void writeMagic( std::ostream & file ) const;

        void writeHeader( std::ostream & file ) const;

        void writeSurfaceData( std::ostream & file ) const;

        uint32_t magic{ 0 };

        struct
        {
            uint32_t size;
            uint32_t flags;
            uint32_t height;
            uint32_t width;
            uint32_t pitch;
            uint32_t depth;
            uint32_t mipmaps;
            uint32_t reserved1[11];

            struct
            {
                uint32_t size;
                uint32_t flags;
                char fourCC[4];
                uint32_t bits;
                uint32_t redBitMask;
                uint32_t greenBitMask;
                uint32_t blueBitMask;
                uint32_t alphaBitMask;
            } pixelFormat;

            uint32_t caps;
            uint32_t caps2;
            uint32_t caps3;
            uint32_t caps4;
            uint32_t reserved2;
        } header;

        Surface surface;
    };

    class DirectDrawSurface::BadFile : public std::runtime_error
    {
    public:
        BadFile() :
            std::runtime_error( "bad file" )
        {}
    };

    class DirectDrawSurface::InvalidType : public std::runtime_error
    {
    public:
        InvalidType() :
            std::runtime_error( "invalid type" )
        {}
    };

    class DirectDrawSurface::BadBitmap : public std::runtime_error
    {
    public:
        BadBitmap() :
            std::runtime_error( "bad bitmap" )
        {}
    };

}

#endif
