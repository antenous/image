/*
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

    class DirectDrawSurface
    {
    public:
        class BadFile;

        class InvalidType;

        class BadBitmap;

        typedef std::vector< uint32_t > Surface;

        virtual ~DirectDrawSurface() = default;

        void loadFrom( std::istream & file );

        void convertFrom( const Bitmap & bitmap );

        void saveTo( std::ostream & file ) const;

        virtual uint32_t getHeight() const;

        virtual uint32_t getWidth() const;

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
