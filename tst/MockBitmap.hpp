/*
 * MockBitmap.hpp
 *
 *  Created on: Oct 4, 2016
 *      Author: Antero Nousiainen
 */

#ifndef MOCKBITMAP_HPP_
#define MOCKBITMAP_HPP_

#include "Bitmap.hpp"
#include <gmock/gmock.h>

namespace image
{

    class MockBitmap : public Bitmap
    {
    public:
        MOCK_CONST_METHOD0( getHeight, int32_t() );

        MOCK_CONST_METHOD0( getWidth, int32_t() );

        MOCK_CONST_METHOD0( getPalette, Palette() );
    };

}

#endif
