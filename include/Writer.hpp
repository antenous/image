/*
 * Writer.hpp
 *
 *  Created on: Dec 19, 2017
 *      Author: Antero Nousiainen
 */

#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <ostream>

namespace image
{

    struct Writer
    {
        template<typename... Args>
        static void write(std::ostream & to, std::tuple<Args...> && tuple)
        {
            write(to, tuple, std::index_sequence_for<Args...>{});
        }

        template<typename Tuple, std::size_t... I>
        static void write(std::ostream & to, const Tuple & t, std::index_sequence<I...>)
        {
            // Brace-enclosed initializers
            // http://en.cppreference.com/w/cpp/language/parameter_pack
            // "since initializer lists guarantee sequencing, this can be used
            // to call a function on each element of a pack, in order:"
            using foreach = int[];
            foreach{ (write(to, std::get<I>(t)), 0)... };
        }

        template<typename T>
        static void write(std::ostream & stream, T && t)
        {
            stream.write(reinterpret_cast<const char*>(&t), sizeof(t));
        }
    };

}

#endif
