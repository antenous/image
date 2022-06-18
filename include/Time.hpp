/*
 * @file include/Time.hpp
 * Time.hpp
 *
 *  Created on: May 5, 2020
 *      Author: Antero Nousiainen
 */

#ifndef TIME_HPP_
#define TIME_HPP_

#include <chrono>
#include <functional>
#include <type_traits>

namespace image
{

    /**
        @brief  Measure the time it takes to run a callable

        Time runs a callable with any given arguments and measures how long it took

        @code

        Time<std::chrono::high_resolution_clock> time;
        const auto [result, elapsed] = time(BitmapReader::read, std::ifstream("img.bmp", std::ios::binary));
        std::cout
            << "File read in " << std::fixed << std::setprecision(3)
            << std::chrono::duration<float, std::milli>(elapsed).count() << "ms" << std::endl;

        @endcode
    */
    template<typename Clock>
    class Time
    {
    public:
        using duration = typename Clock::duration;

        using time_point = typename Clock::time_point;

        /**
            @brief      Times a callable with a return value
            @return     Result of callable paired with elapsed time
        */
        template<typename F, typename... Args>
        std::enable_if_t<!std::is_void_v<std::invoke_result_t<F, Args &&...>>,
            std::pair<std::invoke_result_t<F, Args &&...>, duration>>
        operator()(F && f, Args &&... args) const
        {
            const auto start = Clock::now();
            const auto result = std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
            const auto stop = Clock::now();
            return std::make_pair(result, stop - start);
        }

        /**
            @brief      Times a callable without a return value
            @return     Elapsed time
        */
        template<typename F, typename... Args>
        std::enable_if_t<std::is_void_v<std::invoke_result_t<F, Args &&...>>, duration>
        operator()(F && f, Args &&... args) const
        {
            const auto start = Clock::now();
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
            const auto stop = Clock::now();
            return (stop - start);
        }
    };

}

#endif
