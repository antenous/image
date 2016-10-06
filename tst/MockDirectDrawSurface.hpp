/*
 * MockDirectDrawSurface.hpp
 *
 *  Created on: Oct 7, 2016
 *      Author: antsa
 */

#ifndef MOCKDIRECTDRAWSURFACE_HPP_
#define MOCKDIRECTDRAWSURFACE_HPP_

#include "DirectDrawSurface.hpp"
#include <gmock/gmock.h>

namespace image
{

    class MockDirectDrawSurface : public DirectDrawSurface
    {
    public:
        MOCK_CONST_METHOD0( getHeight, uint32_t() );

        MOCK_CONST_METHOD0( getWidth, uint32_t() );

        MOCK_CONST_METHOD0( getSurface, Surface() );
    };

}

#endif
