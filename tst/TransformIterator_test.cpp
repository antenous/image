/*
 * TransformIterator_test.cpp
 *
 *  Created on: Jun 11, 2019
 *      Author: Antero Nousiainen
 */

#include "TransformIterator.hpp"
#include <vector>
#include <gtest/gtest.h>

using namespace image;
using namespace testing;

namespace
{
    class TransformIteratorTest : public Test
    {
    protected:
        using Range = std::vector<int>;

        struct Widget
        {
            int id;
            std::string name;
        };
    };
}

TEST_F(TransformIteratorTest, IsOutputIterator)
{
    using TransformIterator = TransformIterator<Range::iterator, std::function<int(int)>>;

    EXPECT_TRUE((std::is_same_v<
        std::output_iterator_tag,
        std::iterator_traits<TransformIterator>::iterator_category>));
}

TEST_F(TransformIteratorTest, TransformWithFunctor)
{
    const Range range{1, 2, 3};
    Range result;

    const auto square([](auto i){ return i*i; });
    std::copy(
        std::begin(range), std::end(range),
        TransformIterator(std::back_inserter(result), square));

    EXPECT_EQ(Range({1, 4, 9}), result);
}

TEST_F(TransformIteratorTest, TransformWithClassMember)
{
    const Widget widgets[]{{13, "foo"}, {42, "bar"}};
    Range result;

    std::copy(
        std::begin(widgets), std::end(widgets),
        TransformIterator(std::back_inserter(result), &Widget::id));

    EXPECT_EQ(Range({13, 42}), result);
}
