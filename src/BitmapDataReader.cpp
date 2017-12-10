/*
 * BitmapDataReader.cpp
 *
 *  Created on: May 5, 2017
 *      Author: Antero Nousiainen
 */

#include "BitmapDataReader.hpp"
#include <istream>
#include <utility>

using namespace image;

namespace
{
    uint8_t countPadding( uint16_t bits, int32_t width )
    {
        const auto bytesInRow(( bits * width + 31 ) / 32 * 4 );
        return bytesInRow - width * 3;
    }
}

BitmapDataReader::BitmapDataReader( std::istream & file, int32_t width, int32_t height ) :
    file( &file ),
    width( width ),
    height( height )
{}

auto BitmapDataReader::begin() -> LineIterator
{
    if ( !file )
        throw SubsequentCallToBegin();

    return LineIterator( *std::exchange( file, nullptr ), width );
}

auto BitmapDataReader::end() const -> LineIterator
{
    return LineIterator( height );
}

BitmapDataReader::LineIterator::LineIterator( std::istream & file, int32_t width ) :
    file( &file ),
    padding( countPadding( 24, width )),
    lineBuffer( width * 3 )
{}

BitmapDataReader::LineIterator::LineIterator( int32_t height ) :
    height( height )
{}

bool BitmapDataReader::LineIterator::operator!=( const LineIterator & other ) const
{
    return height != other.height;
}

const BitmapDataReader::LineIterator & BitmapDataReader::LineIterator::operator++()
{
    ++height;
    return *this;
}

const std::vector< uint8_t > & BitmapDataReader::LineIterator::operator*()
{
    for ( ; lineInBuffer != height; ++lineInBuffer )
        readLine();

    return lineBuffer;
}

void BitmapDataReader::LineIterator::readLine()
{
    file->read( reinterpret_cast< char* >( &lineBuffer[0] ), lineBuffer.size() );
    skipPadding();
}

void BitmapDataReader::LineIterator::skipPadding()
{
    file->seekg( padding, file->cur );
}
