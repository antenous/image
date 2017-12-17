/*
 * main.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: Antero Nousiainen
 */

#include <fstream>
#include <iostream>
#include "BitmapReader.hpp"
#include "BitmapWriter.hpp"
#include "DirectDrawSurfaceReader.hpp"
#include "DirectDrawSurfaceWriter.hpp"
#include "ImageConverter.hpp"

using namespace image;

namespace
{
    int help(const char * name)
    {
        std::cout << "usage: " << name << " <bmp|dds file>" << std::endl;
        return 0;
    }

    template<typename Reader, typename Writer>
    void convert(const char * in, const char * out)
    {
        Writer::write(std::ofstream(out, std::ios::binary),
            ImageConverter::convert(
                Reader::read(std::ifstream(in, std::ios::binary))));
    }
}

int main(int argc, char * argv[])
{
    if (argc != 2)
        return help(argv[0]);

    try
    {
        try
        {
            convert<BitmapReader, DirectDrawSurfaceWriter>(argv[1], "out.dds");
        }
        catch (const Bitmap::InvalidType &)
        {
            convert<DirectDrawSurfaceReader, BitmapWriter>(argv[1], "out.bmp");
        }
    }
    catch (const std::runtime_error & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
