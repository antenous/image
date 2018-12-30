/*
 * @file include/BlockIterator.hpp
 * BlockIterator.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: Antero Nousiainen
 */

#ifndef BLOCKITERATOR_HPP_
#define BLOCKITERATOR_HPP_

#include <cstdint>
#include <iterator>

namespace image
{

    /**
        Iterator adaptor for block traversal

        Bitmap views the image as a continuous stream of colors starting from the
        bottom left corner of the image while the direct draw surface sees the image
        as a stream of 4x4 blocks of texels starting from the top right corner.

               56 57 58 59 | 60 61 62 63 -> bmp
               48 49 50 51 | 52 53 54 55
               40 41 42 43 | 44 45 46 47
        dds -> 32 33 34 35 | 36 37 38 39
               ------------+------------
               24 25 26 27 | 28 29 30 31 -> dds
               16 17 18 19 | 20 21 22 23
                8  9 10 11 | 12 13 14 15
        bmp ->  0  1  2  3 |  4  5  6  7

        To work with the different color streams an iterator capable of traversing
        between the two is needed to avoid copying data from one format to the other.
    */
    template<typename Iterator>
    class BlockIterator
    {
    public:
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using pointer = void;
        using reference = typename Iterator::reference;
        using value_type = typename Iterator::value_type;

        BlockIterator(Iterator first, int32_t height, int32_t width) noexcept:
            xCell(1, 4),
            yCell(width, 4),
            xBlock(4, width/4),
            yBlock(-4*width, height/4)
        {
            xCell = yCell = xBlock = yBlock = std::next(first, (height - 4)*width);
        }

        BlockIterator(int32_t height, int32_t width) noexcept :
            steps(height*width)
        {}

        bool operator!=(const BlockIterator & rhs) const noexcept
        {
            return steps != rhs.steps;
        }

        BlockIterator & operator++() noexcept
        {
            ++steps;
            ++xCell;

            if (!xCell)
                xCell = ++yCell;

            if (!yCell)
                xCell = yCell = ++xBlock;

            if (!xBlock)
                xCell = yCell = xBlock = ++yBlock;

            return *this;
        }

        BlockIterator operator++(int) noexcept
        {
            BlockIterator tmp(*this);
            operator++();
            return tmp;
        }

        reference operator*() noexcept
        {
            return *xCell;
        }

    private:
        class Stepper
        {
        public:
            Stepper() = default;

            Stepper(int step, int retireAfter) noexcept :
                step(step),
                retireAfter(retireAfter)
            {}

            Iterator & operator=(const Iterator & it) noexcept
            {
                steps = 0;
                return this->it = it;
            }

            Iterator & operator++() noexcept
            {
                ++steps;
                return it = std::next(it, step);
            }

            explicit operator bool() const noexcept
            {
                return steps < retireAfter;
            }

            reference operator*() noexcept
            {
                return *it;
            }

        private:
            Iterator it;
            int step{ 0 };
            int steps{ 0 };
            int retireAfter{ 0 };
        };

        Stepper xCell;
        Stepper yCell;
        Stepper xBlock;
        Stepper yBlock;
        int32_t steps{ 0 };
    };

    /**
        Convenience function template that constructs a deblocking iterator
    */
    template<typename Container>
    auto deblocker(Container & container, int32_t height, int32_t width)
    {
        return BlockIterator<typename Container::iterator>(container.begin(), height, width);
    }

}

#endif
