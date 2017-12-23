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
            // Brace-enclosed initializers
            // http://en.cppreference.com/w/cpp/language/parameter_pack
            // "since initializer lists guarantee sequencing, this can be used
            // to call a function on each element of a pack, in order:"
            using foreach = int[];
            foreach{ (read(from, std::get<I>(t)), 0)... };
        }

        template<typename T>
        static void read(std::istream & stream, T && t)
        {
            stream.read(reinterpret_cast<char*>(&t), sizeof(t));
        }
    };

}

#endif
