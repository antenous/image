/*
 * @file include/TransformIterator.hpp
 * TransformIterator.hpp
 *
 *  Created on: Jun 11, 2019
 *      Author: Antero Nousiainen
 */

#ifndef TRANSFORMITERATOR_HPP_
#define TRANSFORMITERATOR_HPP_

#include <functional>
#include <iterator>
#include "Pipeable.hpp"

namespace image
{

    /**
        @brief Iterator adaptor for output transform

        Transform iterator adaptor is a counterpart for the std::transform
        algorithm; it can be used together with the stl algorithms to transform
        the original range into something new. It is especially useful with
        algorithms that generate more items that are in the original range
        (e.g. decompression algorithms).

        @code
            struct Widget
            {
                int id;
                std::string name;
            };

            const Widget widgets[]{{13, "foo"}, {42, "bar"}};
            std::vector<int> ids;

            std::copy(
                std::begin(widgets), std::end(widgets),
                std::back_inserter(ids) | transformed(&Widget::id));
        @endcode
    */
    template<typename Iterator, typename UnaryFunction>
    class TransformIterator
    {
    public:
        using difference_type = ptrdiff_t;
        using iterator_category = std::output_iterator_tag;
        using pointer = void;
        using reference = void;
        using value_type = void;

        TransformIterator(const Iterator & iterator, UnaryFunction f) noexcept:
            iterator(iterator),
            f(std::move(f))
        {}

        TransformIterator & operator*() noexcept
        {
            return *this;
        }

        template<typename T>
        TransformIterator & operator=(T && t)
        {
            *iterator = std::invoke(f, std::forward<T>(t));
            return *this;
        }

        TransformIterator operator++(int) noexcept
        {
            TransformIterator tmp(*this);
            ++*this;
            return tmp;
        }

        TransformIterator & operator++() noexcept
        {
            ++iterator;
            return *this;
        }

    private:
        Iterator iterator;
        UnaryFunction f;
    };

    template<typename UnaryFunction>
    auto transformed(UnaryFunction f)
    {
        return Pipeable{[f = std::move(f)](auto it)
            {
                return TransformIterator(it, std::move(f));
            }};
    }

}

#endif
