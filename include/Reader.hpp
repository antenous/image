/*
 * Reader.hpp
 *
 *  Created on: Dec 19, 2017
 *      Author: Antero Nousiainen
 */

#ifndef READER_HPP_
#define READER_HPP_

#include <istream>
#include <tuple>
#include <utility>

namespace image
{

    struct Reader
    {
        template<typename... Args>
        static void read(std::istream & from, std::tuple<Args&...> && tuple)
        {
            read(from, tuple, std::index_sequence_for<Args...>{});
        }

        template<typename Tuple, std::size_t... I>
        static void read(std::istream & from, const Tuple & t, std::index_sequence<I...>)
        {
            (read(from, std::get<I>(t)), ...);
        }

        template<typename T>
        static void read(std::istream & stream, T && t)
        {
            stream.read(reinterpret_cast<char*>(&t), sizeof(t));
        }
    };

}

#endif
