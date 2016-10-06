/*
 * BitmapConverter.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Antero Nousiainen
 */

#include "BitmapConverter.hpp"
#include <algorithm>
#include <initializer_list>

using namespace image;

namespace
{
    class MultiplesTester
    {
    public:
        MultiplesTester( int32_t value ) :
            value( value )
        {}

        MultiplesTester( const BitmapConverter::Palette & palette ) :
            MultiplesTester( palette.size() )
        {}

        MultiplesTester( const DirectDrawSurface::Surface & surface ) :
            MultiplesTester( surface.size() )
        {}

        bool isMultipleOf( uint8_t divisor ) const
        {
            return ( value && value % divisor == 0 );
        }

    private:
        int32_t value;
    };

    void convertIntoDistanceFromReference( uint16_t reference, BitmapConverter::Color & color )
    {
        std::transform( color.begin(), color.end(), color.begin(),
            [&reference]( uint16_t color ){ return std::abs( reference - color ); });
    }

    uint8_t distanceToBitIndex( uint16_t dist )
    {
        return dist < 2 ? !dist : dist == 2 ? 3 : 2;
    }

    uint8_t closestMatch( uint16_t reference, BitmapConverter::Color color )
    {
        convertIntoDistanceFromReference( reference, color );
        return distanceToBitIndex( std::distance( color.begin(), std::min_element( color.begin(), color.end() )));
    }

    bool isValidSize( MultiplesTester height, MultiplesTester width, MultiplesTester palette )
    {
        return height.isMultipleOf( 4 ) && width.isMultipleOf( 4 ) && palette.isMultipleOf( 16 );
    }

    bool isValidSize( MultiplesTester surface )
    {
        return surface.isMultipleOf( 2 );
    }

    uint32_t referenceColors( const BitmapConverter::Color & color )
    {
        return static_cast< uint32_t >( color[0] ) << 16 | color[1];
    }

    void countIntermediateColors( BitmapConverter::Color & color )
    {
        color[2] = color[0] * 2 / 3 + color[1] / 3;
        color[3] = color[0] / 3 + color[1] * 2 / 3;
    }

    uint8_t downscaleSubColor( uint32_t color, uint8_t bits )
    {
        return ( color & 0xff ) >> ( 8 - bits );
    }

    uint8_t upscaleSubColor( uint16_t color, uint8_t bits )
    {
        uint8_t subColor(( color << ( 8 - bits )) & 0xff );
        return subColor | ( subColor >> bits );
    }
}

BitmapConverter::Converted BitmapConverter::convert( const Bitmap & bitmap ) const
{
    return convertBlocks( rearrangePaletteToBlocks(
        bitmap.getHeight(), bitmap.getWidth(), trueToHigh( bitmap.getPalette() )));
}

BitmapConverter::Palette BitmapConverter::trueToHigh( const Bitmap::Palette & truePalette ) const
{
    Palette highPalette( truePalette.size() );

    std::transform(
        truePalette.begin(), truePalette.end(), highPalette.begin(),
        [this]( uint32_t color ){ return trueToHigh( color ); });

    return std::move( highPalette );
}

uint16_t BitmapConverter::trueToHigh( uint32_t trueColor ) const
{
    uint16_t highColor( 0 );

    for ( auto bitsToKeep : { 5, 6, 5 })
    {
        highColor <<= bitsToKeep;
        highColor |= downscaleSubColor( trueColor, bitsToKeep );
        trueColor >>= 8;
    }

    return highColor;
}

BitmapConverter::Palette BitmapConverter::rearrangePaletteToBlocks(
    int32_t height, int32_t width, const Palette & palette ) const

{
    if ( !isValidSize( height, width, palette ))
        throw BadSize();

    Palette rearranged( palette.size() );
    auto it( rearranged.begin() );

    for ( int32_t y( height - 4 ); y >= 0; y -= 4 )
        for ( int32_t x( 0 ); x < width; x += 4 )
            for ( auto i( y*width + x ), end( i + width*4 ); i < end; i += width )
                for ( auto j( i ), end( j + 4 ); j < end; ++j)
                    *it++ = palette[j];

    return std::move( rearranged );
}

BitmapConverter::Palette BitmapConverter::rearrangeBlocksToPalette(
    int32_t height, int32_t width, const Palette & blocks ) const

{
    if ( !isValidSize( height, width, blocks ))
        throw BadSize();

    Palette rearranged( blocks.size() );
    auto it( blocks.begin() );

    for ( int32_t y( height - 4 ); y >= 0; y -= 4 )
        for ( int32_t x( 0 ); x < width; x += 4 )
            for ( auto i( y*width + x ), end( i + width*4 ); i < end; i += width )
                for ( auto j( i ), end( j + 4 ); j < end; ++j)
                    rearranged[j] = *it++;

    return std::move( rearranged );
}

BitmapConverter::Converted BitmapConverter::convertBlocks( const Palette & blocks ) const
{
    Converted result;

    for ( auto first( blocks.begin() ), last( blocks.end() ); first != last; std::advance( first, 16 ))
    {
        Block block;
        std::copy_n( first, 16, block.begin() );
        auto converted( convertBlock( block ));
        result.insert( result.end(), converted.begin(), converted.end() );
    }

    return std::move( result );
}

std::array< uint32_t, 2 > BitmapConverter::convertBlock( const Block & block ) const
{
    const auto color( createColorTable( block ));
    const auto lookup( createLookupTable( color, block ));
    return { referenceColors( color ), lookup };
}

BitmapConverter::Color BitmapConverter::createColorTable( const Block & block ) const
{
    Color color;
    color[0] = *std::min_element( block.begin(), block.end() );
    color[1] = *std::max_element( block.begin(), block.end() );

    countIntermediateColors( color );
    return std::move( color );
}

BitmapConverter::Color BitmapConverter::createColorTable( uint32_t referenceColors ) const
{
    Color color;
    color[0] = referenceColors >> 16;
    color[1] = referenceColors & 0xffff;

    countIntermediateColors( color );
    return std::move( color );
}

BitmapConverter::Converted BitmapConverter::convert( const DirectDrawSurface & dds ) const
{
    return highToTrue( rearrangeBlocksToPalette( dds.getHeight(), dds.getWidth(), createColorBlocks( dds.getSurface() )));
}

uint32_t BitmapConverter::createLookupTable( const Color & color, const Block & block ) const
{
    uint32_t lookup( 0 );

    for ( unsigned y( 0 ), x; y < 4; ++y )
        for ( x = 0, lookup <<= 8; x < 4; ++x )
            lookup |= closestMatch( block[y*4 + x], color ) << x*2;

    return lookup;
}

BitmapConverter::Palette BitmapConverter::createColorBlocks( const DirectDrawSurface::Surface & surface ) const
{
    if ( !isValidSize( surface ))
        throw BadSize();

    Palette palette;

    for ( auto first( surface.begin() ), last( surface.end() ); first != last; std::advance( first, 2 ))
    {
        const auto block( createColorBlock( createColorTable( *first ), *std::next( first )));
        palette.insert( palette.end(), block.begin(), block.end() );
    }

    return palette;
}

BitmapConverter::Block BitmapConverter::createColorBlock( const Color & color, uint32_t lookupTable ) const
{
    Block block;

    for ( int y( 3 ); y >= 0; --y )
        for ( unsigned x( 0 ); x < 4; ++x, lookupTable >>= 2 )
            block[y*4 + x] = color[distanceToBitIndex( lookupTable & 0b11 )];

    return block;
}

BitmapConverter::Converted BitmapConverter::highToTrue( const Palette & palette ) const
{
    Converted converted( palette.size() );
    auto it( converted.begin() );

    for ( auto p : palette )
        *it++ = highToTrue( p );

    return converted;
}

uint32_t BitmapConverter::highToTrue( uint16_t highColor ) const
{
    uint32_t trueColor( 0 );

    for ( auto bitsToKeep : { 5, 6, 5 })
    {
        trueColor <<= 8;
        trueColor |= upscaleSubColor( highColor, bitsToKeep );
        highColor >>= bitsToKeep;
    }

    return trueColor;
}
