/*
 * BlockIterator_test.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: Antero Nousiainen
 */

#include "BlockIterator.hpp"
#include <type_traits>
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class BlockIteratorTest : public Test
    {
    protected:
        template<typename Container>
        Container block(const Container & in, int32_t height, int32_t width)
        {
            Container out;
            out.reserve(in.size());
            std::copy(
                BlockIterator<typename Container::const_iterator>(in.begin(), height, width),
                BlockIterator<typename Container::const_iterator>(height, width),
                std::back_inserter(out));
            return out;
        }

        template<typename Container>
        Container deblock(const Container & in, int32_t height, int32_t width)
        {
            Container out(in.size());
            std::copy(in.begin(), in.end(), deblocker(out, height, width));
            return out;
        }

        template<typename Container>
        Container deblock(const Container & in)
        {
            Container out(in);
            std::sort(out.begin(), out.end());
            return out;
        }

        const std::vector<int> fourByFour{{
             0,  1,  2,  3,
             4,  5,  6,  7,
             8,  9, 10, 11,
            12, 13, 14, 15 }};

        const std::vector<int> eightByFour{{
             0,  1,  2,  3,
             8,  9, 10, 11,
            16, 17, 18, 19,
            24, 25, 26, 27,
             4,  5,  6,  7,
            12, 13, 14, 15,
            20, 21, 22, 23,
            28, 29, 30, 31 }};

        const std::vector<int> fourByEight{{
            16, 17, 18, 19,
            20, 21, 22, 23,
            24, 25, 26, 27,
            28, 29, 30, 31,
             0,  1,  2,  3,
             4,  5,  6,  7,
             8,  9, 10, 11,
            12, 13, 14, 15 }};
    };
}

TEST_F(BlockIteratorTest, BlockOneBlock)
{
    EXPECT_EQ(fourByFour, block(fourByFour, 4, 4));
}

TEST_F(BlockIteratorTest, DeblockOneBlock)
{
    EXPECT_EQ(fourByFour, deblock(fourByFour, 4, 4));
}

TEST_F(BlockIteratorTest, BlockTwoBlocksSideBySide)
{
    EXPECT_EQ(eightByFour, block(deblock(eightByFour), 4, 8));
}

TEST_F(BlockIteratorTest, DeblockTwoBlocksSideBySide)
{
    EXPECT_EQ(deblock(eightByFour), deblock(eightByFour, 4, 8));
}

TEST_F(BlockIteratorTest, BlockTwoBlocksOneOnTheOther)
{
    EXPECT_EQ(fourByEight, block(deblock(fourByEight), 8, 4));
}

TEST_F(BlockIteratorTest, DeblockTwoBlocksOneOnTheOther)
{
    EXPECT_EQ(deblock(fourByEight), deblock(fourByEight, 8, 4));
}
