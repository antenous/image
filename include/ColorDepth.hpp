/*
 * @file include/ColorDepth.hpp
 * ColorDepth.hpp
 *
 *  Created on: Oct 9, 2016
 *      Author: Antero Nousiainen
 */

#ifndef COLORDEPTH_HPP_
#define COLORDEPTH_HPP_

#include <algorithm>
#include <cstdint>
#include <memory>

namespace image
{

    /**
        @brief  Class for converting color depths
    */
    class ColorDepth
    {
    public:
        /**
            @brief Convert a 24bit true color into a 16bit high color

            Convert a 24bit 8:8:8 BGR color into a 16bit 5:6:5 RGB color by
            dropping the least significant bits of each of the color component.

            @param trueColor    24bit true color to convert
            @return 16bit high color
        */
        static uint16_t trueToHigh( uint32_t trueColor );


        /**
            @brief Convert a 24bit color range into a 16bit color range

            @param in   24bit color range to convert
            @return 16bit color range
        */
        template<
            template< class, class > class Range, typename Alloc,
            typename AllocOut = std::allocator< uint16_t >>
        static Range< uint16_t, AllocOut > trueToHigh( const Range< uint32_t, Alloc > & in )
        {
            Range< uint16_t, AllocOut > out( in.size() );

            std::transform(
                in.begin(), in.end(), out.begin(),
                []( uint32_t color ){ return trueToHigh( color ); });

            return std::move( out );
        }

        /**
            @brief Convert a 16bit high color into a 24bit true color

            Convert a 16bit 5:6:5 RGB color into a 24bit 8:8:8 BGR color by
            adding the most significant bits of each of the color components
            to their least significant bits.

            @param highColor    16bit high color to convert
            @return 24 bit true color
        */
        static uint32_t highToTrue( uint16_t highColor );

        /**
            @brief Convert a 16bit color range into a 24bit color range

            @param in   16bit color range to convert
            @return 24bit color range
        */
        template<
            template< class, class > class Range, typename Alloc,
            typename AllocOut = std::allocator< uint32_t >>
        static Range< uint32_t, AllocOut > highToTrue( const Range< uint16_t, Alloc > & in )
        {
            Range< uint32_t, AllocOut > out( in.size() );

            std::transform(
                in.begin(), in.end(), out.begin(),
                []( uint16_t color ){ return highToTrue( color ); });

            return std::move( out );
        }
    };

}

#endif
