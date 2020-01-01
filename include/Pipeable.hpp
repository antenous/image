/*
 * @file include/Pipeable.hpp
 * Pipeable.hpp
 *
 *  Created on: Dec 31, 2019
 *      Author: Antero Nousiainen
 */

#ifndef PIPEABLE_HPP_
#define PIPEABLE_HPP_

#include <functional>

namespace image
{

    /**
        @brief  Helper for creating pipelines

        @code
            Pipeable squared{[](auto i){ return i*i; }};
            int i = 3 | squared; // i = 9
        @endcode
    */
    template<typename... Ts> struct Pipeable : Ts... { using Ts::operator()...; };
    template<typename... Ts> Pipeable(Ts...) -> Pipeable<Ts...>;

    template<typename Lhs, typename... Ts>
    decltype(auto) operator|(Lhs && lhs, const Pipeable<Ts...> & pipeable)
    {
        return std::invoke(pipeable, std::forward<Lhs>(lhs));
    }

}

#endif
