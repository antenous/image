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

    /**
        Class to hold all the data about a direct draw surface image
    */
    class DirectDrawSurface
    {
    public:
        class BadFile;

        class InvalidType;

        typedef std::vector< uint32_t > Surface;

        /**
            Check if holds a valid direct draw surface image

            @return true if holds a valid direct draw surface image
        */
        explicit operator bool() const;

        /**
            Load a direct draw surface image from a file

            @param file File to load the image from

            @throw BadFile if unable to load from the file
            @throw InvalidType if file is not a direct draw surface image
        */
        void load( std::istream & file );

        /**
            Save the loaded image to a file

            @param file File to load to

            @throw BadFile if unable to save to the file
            @throw InvalidType if the direct draw surface has not been loaded
        */
        void save( std::ostream & file ) const;

        /**
            Return height of the image

            @return Height of the image
        */
        uint32_t getHeight() const;

        /**
             Return width of the image

             @return Width of the image
        */
        uint32_t getWidth() const;

        /**
            Return the image surface data

            @return Surface data
        */
        Surface getSurface() const;

    private:
        friend class ImageConverter;

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

}

#endif
