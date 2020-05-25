/*
 * Writer.hpp
 *
 *  Created on: Dec 19, 2017
 *      Author: Antero Nousiainen
 */

#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <tuple>
#include <ostream>
#include <utility>

namespace image
{

    struct Writer
    {
        template<typename... Args>
        static void write(std::ostream & to, const std::tuple<Args...> & tuple)
        {
            write(to, tuple, std::index_sequence_for<Args...>{});
        }

        template<typename Tuple, std::size_t... I>
        static void write(std::ostream & to, const Tuple & t, std::index_sequence<I...>)
        {
            (write(to, std::get<I>(t)), ...);
        }

        template<typename T>
        static void write(std::ostream & stream, const T & t)
        {
            stream.write(reinterpret_cast<const char*>(&t), sizeof(t));
        }
    };

}

#endif
