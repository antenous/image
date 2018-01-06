/*
 * Bitmap_test.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: Antero Nousiainen
 */

#include "Bitmap.hpp"
#include <gtest/gtest.h>
#include "BitmapReader.hpp"

using namespace image;
using namespace testing;

namespace
{

    class BitmapTest : public Test
    {
    protected:
        void writeFileHeader()
        {
            const char type[]{ 'B', 'M' };
            const uint32_t reserved( 0 );
            const uint32_t offset( 14 + 40 );
            const uint32_t size( offset + 16 );

            writeToFile( type );
            writeToFile( size );
            writeToFile( reserved );
            writeToFile( offset );
        }

        void writeInfoHeader()
        {
            const uint32_t size( 40 );
            const int32_t width( 2 );
            const int32_t height( 2 );
            const uint16_t planes( 1 );
            const uint16_t bits( 24 );
            const uint32_t compression( 0 );
            const uint32_t imageSize( 16 );
            const uint32_t horizontalResolution( 0 );
            const uint32_t verticalResolution( 0 );
            const uint32_t colors( 0 );
            const uint32_t importantColors( 0 );

            writeToFile( size );
            writeToFile( width );
            writeToFile( height );
            writeToFile( planes );
            writeToFile( bits );
            writeToFile( compression );
            writeToFile( imageSize );
            writeToFile( horizontalResolution );
            writeToFile( verticalResolution );
            writeToFile( colors );
            writeToFile( importantColors );
        }

        void writeColorTable()
        {
            createRedPixel();
            createWhitePixel();
            createPadding();
            createBluePixel();
            createGreenPixel();
            createPadding();
        }

        void createRedPixel()
        {
            writeToFile({ 0, 0, 255 });
        }

        void createWhitePixel()
        {
            writeToFile({ 255, 255, 255 });
        }

        void createPadding()
        {
            writeToFile({ 0, 0 });
        }

        void createBluePixel()
        {
            writeToFile({ 255, 0 , 0 });
        }

        void createGreenPixel()
        {
            writeToFile({ 0, 255, 0 });
        }

        template< typename T >
        void writeToFile( const T & t )
        {
            fileIn.write( reinterpret_cast< const char* >( &t ), sizeof( t ));
        }

        void writeToFile( const std::vector< uint8_t > & bytes )
        {
            for ( auto byte : bytes )
                writeToFile( byte );
        }

        bool hasUnreadData( std::stringstream & file ) const
        {
            return file.peek() != std::istream::traits_type::eof();
        }

        void loadBitmapFromFile()
        {
            writeFileHeader();
            writeInfoHeader();
            writeColorTable();
            bitmap = BitmapReader::read(std::move(fileIn));
        }

        Bitmap bitmap;
        std::stringstream fileIn;
    };

}

TEST_F( BitmapTest, WhenNotLoaded_EvaluatesToFalse )
{
    EXPECT_FALSE( bitmap );
}

TEST_F( BitmapTest, GivenValidFile_WhenLoaded_ReadsFile )
{
    loadBitmapFromFile();

    EXPECT_FALSE( hasUnreadData( fileIn ));
}

TEST_F( BitmapTest, WhenBitmapIsLoaded_HeightIsGettable )
{
    loadBitmapFromFile();

    EXPECT_EQ( 2, bitmap.getHeight() );
}

TEST_F( BitmapTest, WhenBitmapIsLoaded_WidthIsGettable )
{
    loadBitmapFromFile();

    EXPECT_EQ( 2, bitmap.getWidth() );
}

TEST_F( BitmapTest, WhenBitmapIsLoaded_PaletteIsGettable )
{
    loadBitmapFromFile();

    EXPECT_EQ( 4, bitmap.getColors().size() );
}
