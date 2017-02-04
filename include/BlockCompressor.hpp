/*
 * @file include/BlockCompressor.hpp
 * BlockCompressor.hpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Antero Nousiainen
 */

#ifndef BLOCKCOMPRESSOR_HPP_
#define BLOCKCOMPRESSOR_HPP_

#include <algorithm>
#include <array>
#include <cstdint>

namespace image
{

    /**
        @brief  Class for compressing and decompressing image data
    */
    class BlockCompressor
    {
    public:
        class BadSize;

        /**
            @brief Compress a color range

            @param in   Color range to compress
            @return compressed color range

            @throw BadSize if color range size is not a multiple of 16
        */
        template<
            template< typename, typename > class Range, typename Alloc,
            typename AllocOut = std::allocator< uint32_t >>
        static Range< uint32_t, AllocOut > compress( const Range< uint16_t, Alloc > & in )
        {
            if ( in.size() == 0 || in.size() % 16 != 0 )
                throw BadSize();

            Range< uint32_t, AllocOut > out( in.size() / 16 * 2 );
            auto result( out.begin() );

            for ( auto first( in.begin() ), last( in.end() ); first != last; std::advance( first, 16 ))
                result = compress( first, std::next( first, 16 ), result );

            return out;
        }

        /**
            @brief Decompress a color range

            @param in Color range to compress
            @return decompressed color range

            @throw BadSize if color range size is not a multiple of 2
        */
        template<
            template< typename, typename > class Range, typename Alloc,
            typename AllocOut = std::allocator< uint16_t >>
        static Range< uint16_t, AllocOut > decompress( const Range< uint32_t, Alloc > & in )
        {
            if ( in.size() == 0 || in.size() % 2 != 0 )
                throw BadSize();

            Range< uint16_t, AllocOut > out( in.size() / 2 * 16 );
            auto result( out.begin() );

            for ( auto first( in.begin() ), last( in.end() ); first != last; std::advance( first, 2 ))
                result = decompress( first, std::next( first, 2 ), result );

            return out;
        }

    private:
        typedef std::array< uint16_t, 4 > Color;

        template< typename InputIterator, typename OutputIterator >
        static OutputIterator compress( InputIterator first, InputIterator last, OutputIterator result )
        {
            for ( ; first != last; std::advance( first, 16 ))
                result = compressBlock( first, std::next( first, 16 ), result );

            return result;
        }

        template< typename InputIterator, typename OutputIterator >
        static OutputIterator compressBlock( InputIterator first, InputIterator last, OutputIterator result )
        {
            const auto color( createColorTable( first, last ));
            *result++ = referenceColors( color );
            *result++ = createLookupTable( color, first, last );
            return result;
        }

        template< typename InputIterator >
        static Color createColorTable( InputIterator first, InputIterator last )
        {
            Color color;
            color[0] = *std::max_element( first, last );
            color[1] = *std::min_element( first, last );
            color[2] = interpolate( color[0], color[1] );
            color[3] = interpolate( color[1], color[0] );

            return color;
        }

        static uint16_t interpolate( uint16_t lhs, uint16_t rhs );

        static uint32_t referenceColors( const Color & color );

        template< typename InputIterator >
        static uint32_t createLookupTable( const Color & color, InputIterator first, InputIterator last )
        {
            uint32_t lookup( 0 );

            for ( unsigned x( 0 ); first != last; x = 0 )
                for ( lookup <<= 8; x < 4; ++x )
                    lookup |= findClosest( color, *first++ ) << 2*x;

            return lookup;
        }

        static uint8_t findClosest( Color color, uint16_t ref );

        template< typename InputIterator, typename OutputIterator >
        static OutputIterator decompress( InputIterator first, InputIterator last, OutputIterator result )
        {
            for ( ; first != last; std::advance( first, 2 ))
                result = decompress( createColorTable( *first ), *std::next( first ), result );

            return result;
        }

        static Color createColorTable( uint32_t referenceColors );

        template< typename OutputIterator >
        static OutputIterator decompress( const Color & color, uint32_t lookup, OutputIterator result )
        {
            lookup = reorderBytes( lookup );

            for ( unsigned i( 0 ); i < 16; ++i, lookup >>= 2, ++result )
                *result = color[ lookup & 0b11 ];

            return result;
        }

        static uint32_t reorderBytes( uint32_t lookup );
    };

    class BlockCompressor::BadSize : public std::runtime_error
    {
    public:
        BadSize() :
            std::runtime_error( "bad size" )
        {}
    };

}

#endif
