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
    class MultipleTester
    {
    public:
        MultipleTester( int32_t value ) :
            value( value )
        {}

        MultipleTester( const BitmapConverter::Palette & palette ) :
            MultipleTester( palette.size() )
        {}

        bool isMultipleOf( uint8_t divisor ) const
        {
            return ( value % divisor == 0 );
        }

    private:
        int32_t value;
    };

    void convertIntoDistanceFromReference( uint16_t reference, BitmapConverter::Color & color )
    {
        std::transform( color.begin(), color.end(), color.begin(),
            [&reference]( uint16_t color ){ return std::abs( reference - color ); });
    }

    uint8_t closestMatch( uint16_t reference, BitmapConverter::Color color )
    {
        convertIntoDistanceFromReference( reference, color );
        return std::distance( color.begin(), std::min_element( color.begin(), color.end() ));
    }

    bool isValidSize( MultipleTester height, MultipleTester width, MultipleTester palette )
    {
        return height.isMultipleOf( 4 ) && width.isMultipleOf( 4 ) && palette.isMultipleOf( 16 );
    }

    uint32_t referenceColors( const BitmapConverter::Color & color )
    {
        return static_cast< uint32_t >( color[0] ) << 16 | color[1];
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
        highColor |= ( trueColor & 0xff ) >> ( 8 - bitsToKeep );
        trueColor >>= 8;
    }

    return highColor;
}

uint32_t BitmapConverter::highToTrue( uint16_t highColor ) const
{
    uint32_t trueColor( 0 );

    for ( auto bitsToKeep : { 5, 6, 5 })
    {
        trueColor <<= 8;
        trueColor |= ( highColor << ( 8 - bitsToKeep )) & 0xff;
        highColor >>= bitsToKeep;
    }

    return trueColor;
}

BitmapConverter::Palette BitmapConverter::rearrangePaletteToBlocks(
    int32_t height, int32_t width, const Palette & palette ) const

{
    if ( !isValidSize( height, width, palette ))
        throw BadSize();

    Palette rearranged;

    for ( int32_t y( 0 ); y < height; y += 4 )
        for ( int32_t x( 0 ); x < width; x += 4 )
            for ( auto i( y*width + x ), end( i + width*4 ); i < end; i += width )
                for ( auto j( i ), end( j + 4 ); j < end; ++j)
                    rearranged.push_back( palette[j] );

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
    color[2] = color[0] * 2 / 3 + color[1] / 3;
    color[3] = color[0] / 3 + color[1] * 2 / 3;

    return std::move( color );
}

uint32_t BitmapConverter::createLookupTable( const Color & color, const Block & block ) const
{
    uint32_t lookup( 0 );

    for ( unsigned y( 0 ), x; y < 4; ++y )
        for ( x = 0, lookup <<= 8; x < 4; ++x )
            lookup |= closestMatch( block[y*4 + x], color ) << x*2;

    return lookup;
}
