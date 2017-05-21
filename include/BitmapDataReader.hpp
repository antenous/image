/*
 * @file include/BitmapDataReader.hpp
 * BitmapDataReader.hpp
 *
 *  Created on: May 5, 2017
 *      Author: Antero Nousiainen
 */

#ifndef BITMAPDATAREADER_HPP_
#define BITMAPDATAREADER_HPP_

#include <iosfwd>
#include <stdexcept>
#include <vector>

namespace image
{

    /**
        @brief  Class for reading bitmap color data

        This class is meant to be used with range-based for loops. The bitmap file is read one
        line at a time, padding (if any) at the end of the line is ignored.

        NOTE! Currently only 24bit bitmap files are supported.

        @code
            for ( auto & line : BitmapDataReader( file, 800, 600 ))
                ...
        @endcode
    */
    class BitmapDataReader
    {
    public:
        class SubsequentCallToBegin;

        class LineIterator;

        BitmapDataReader( std::istream & file, int32_t width, int32_t height );

        /**
            @throw SubsequentCallToBegin if begin is called more than once
        */
        auto begin() -> LineIterator;

        auto end() const -> LineIterator;

    private:
        std::istream * file;
        const int32_t width;
        const int32_t height;
    };

    class BitmapDataReader::SubsequentCallToBegin : public std::logic_error
    {
    public:
        SubsequentCallToBegin() :
            std::logic_error( "subsequent call to begin" )
        {}
    };

    class BitmapDataReader::LineIterator
    {
    public:
        LineIterator( std::istream & file, int32_t width );

        LineIterator( int32_t height );

        bool operator!=( const LineIterator & other ) const;

        const LineIterator & operator++();

        const std::vector< uint8_t > & operator*();

    private:
        void readLine();

        void skipPadding();

        std::istream * file{ nullptr };
        int32_t height{ 0 };
        int32_t lineInBuffer{ -1 };
        const std::streamoff padding{ 0 };
        std::vector< uint8_t > lineBuffer;
    };

}

#endif
