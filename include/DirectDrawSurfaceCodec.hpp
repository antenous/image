/*
 * @file include/DirectDrawSurfaceCodec.hpp
 * DirectDrawSurfaceCodec.hpp
 *
 *  Created on: May 22, 2020
 *      Author: Antero Nousiainen
 */

#ifndef DIRECTDRAWSURFACECODEC_HPP_
#define DIRECTDRAWSURFACECODEC_HPP_

#include <stdexcept>
#include "Bitmap.hpp"
#include "DirectDrawSurface.hpp"

namespace image
{

    class DirectDrawSurfaceCodec
    {
    public:
        class BadSize;

        static DirectDrawSurface::Data compress(
            std::int32_t height, std::int32_t width, const Bitmap::Data & data);

        static Bitmap::Data decompress(
            std::uint32_t height, std::uint32_t width, const DirectDrawSurface::Data & data);
    };

    class DirectDrawSurfaceCodec::BadSize : public std::invalid_argument
    {
    public:
        BadSize();
    };

}

#endif
