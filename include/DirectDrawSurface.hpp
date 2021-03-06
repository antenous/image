/*
 * @file include/DirectDrawSurface.hpp
 * DirectDrawSurface.hpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTDRAWSURFACE_HPP_
#define DIRECTDRAWSURFACE_HPP_

#include <vector>
#include "DirectX.hpp"
#include "HighColor.hpp"
#include "Texel.hpp"

namespace image
{

    /**
        Class to hold all the data about a direct draw surface image

        https://docs.microsoft.com/en-us/windows/desktop/direct3ddds/dds-header
    */
    class DirectDrawSurface
    {
    public:
        using Data = std::vector<Texel>;

        using Magic = uint32_t;

        /**
            Return true if image is valid
        */
        explicit operator bool() const;

        static DirectDrawSurface make(std::uint32_t height, std::uint32_t width, const Data & data);

        /**
            Return height of the image
        */
        uint32_t height() const;

        /**
            Return width of the image
        */
        uint32_t width() const;

        Magic magic = 0;

        struct Header
        {
            uint32_t size;
            uint32_t flags;
            uint32_t height;
            uint32_t width;
            uint32_t pitch;
            uint32_t depth;
            uint32_t mipmaps;
            uint32_t reserved1[11];

            struct PixelFormat
            {
                uint32_t size;
                uint32_t flags;
                uint32_t fourCC;
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

        Data data;
    };

}

#endif
