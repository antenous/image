/*
 * main.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: Antero Nousiainen
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "BitmapReader.hpp"
#include "BitmapWriter.hpp"
#include "DirectDrawSurfaceReader.hpp"
#include "DirectDrawSurfaceWriter.hpp"
#include "ImageConverter.hpp"
#include "Time.hpp"

using namespace image;

namespace
{
    Time<std::chrono::high_resolution_clock> time;

    int help(const char * name)
    {
        std::cout << "usage: " << name << " <bmp|dds file>" << std::endl;
        return 0;
    }

    template<typename Rep, typename Period>
    auto format(std::string_view operation, const std::chrono::duration<Rep, Period> & elapsed)
    {
        std::ostringstream formatter;
        formatter
            << std::left << std::setw(11)
            << operation
            << std::right << std::setw(6) << std::fixed << std::setprecision(3)
            << std::chrono::duration<float, std::milli>(elapsed).count() << "ms";
        return formatter.str();
    }

    template<typename Reader>
    auto read(std::istream & file)
    {
        const auto [result, elapsed] = time(Reader::read, file);
        std::cout << format("read", elapsed) << "\n";
        return result;
    }

    auto convert(const Bitmap & bmp)
    {
        const auto [result, elapsed] = time(static_cast<DirectDrawSurface(*)(const Bitmap &)>(ImageConverter::convert), bmp);
        std::cout << format("compress", elapsed) << "\n";
        return result;
    }

    auto convert(const DirectDrawSurface & dds)
    {
        const auto [result, elapsed] = time(static_cast<Bitmap(*)(const DirectDrawSurface &)>(ImageConverter::convert), dds);
        std::cout << format("decompress", elapsed) << "\n";
        return result;
    }

    template<typename Writer, typename T>
    void write(const char * out, T && t)
    {
        std::ofstream file(out, std::ios::binary);
        const auto elapsed = time(Writer::write, file, std::forward<T>(t));
        std::cout << format("write", elapsed) << "\n";
    }

    template<typename Reader, typename Writer>
    void convert(std::istream & in, const char * out)
    {
        write<Writer>(out,
            convert(
                read<Reader>(in)));
    }

    void rewind(std::istream & file)
    {
        file.clear();
        file.seekg(0, file.beg);
    }
}

int main(int argc, char * argv[])
{
    if (argc != 2)
        return help(argv[0]);

    try
    {
        std::ifstream file(argv[1], std::ios::binary);

        try
        {
            convert<BitmapReader, DirectDrawSurfaceWriter>(file, "out.dds");
        }
        catch (const BitmapReader::InvalidType &)
        {
            rewind(file);
            convert<DirectDrawSurfaceReader, BitmapWriter>(file, "out.bmp");
        }
    }
    catch (const std::runtime_error & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
