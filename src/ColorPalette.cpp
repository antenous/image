/*
 * ColorPalette.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: Antero Nousiainen
 */

#include "ColorPalette.hpp"
#include <functional>

using namespace image;

namespace
{
    class MultiplesTester
    {
    public:
        MultiplesTester( int32_t value ) :
            value( value )
        {}

        MultiplesTester( const ColorPalette::Colors & colors ) :
            value( colors.size() )
        {}

        bool isMultipleOf( uint8_t divisor ) const
        {
            return ( value && value % divisor == 0 );
        }

    private:
        const int32_t value;
    };

    bool validDimensions( MultiplesTester height, MultiplesTester width, MultiplesTester colorPalette )
    {
        return height.isMultipleOf( 4 ) && width.isMultipleOf( 4 ) && colorPalette.isMultipleOf( 16 );
    }

    void rearrangeColorPalette(
        int32_t height, int32_t width, const ColorPalette::Colors & colorPalette,
        const std::function< void( int32_t )> & rearrange )
    {
        if ( !validDimensions( height, width, colorPalette ))
            throw ColorPalette::BadDimensions();

        for ( int32_t y( height - 4 ); y >= 0; y -= 4 )
             for ( int32_t x( 0 ); x < width; x += 4 )
                 for ( auto i( y*width + x ), end( i + width*4 ); i < end; i += width )
                     for ( auto j( i ), end( j + 4 ); j < end; ++j )
                         rearrange( j );
    }
}

ColorPalette::Colors ColorPalette::rearrangeForDirectDrawSurface( int32_t height, int32_t width, const Colors & in )
{
    Colors out( in.size() );
    auto it( out.begin() );
    rearrangeColorPalette( height, width, in, [&in, &it]( int32_t j ){ *it++ = in[j]; });
    return std::move( out );
}

ColorPalette::Colors ColorPalette::rearrangeForBitmap(int32_t height, int32_t width, const Colors & in )
{
    Colors out( in.size() );
    auto it( in.cbegin() );
    rearrangeColorPalette( height, width, in, [&it, &out]( int32_t j ){ out[j] = *it++; });
    return std::move( out );
}
