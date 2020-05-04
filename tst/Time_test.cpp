/*
 * Time_test.cpp
 *
 *  Created on: May 5, 2020
 *      Author: Antero Nousiainen
 */

#include "Time.hpp"
#include <gmock/gmock.h>

using namespace image;
using namespace testing;
using namespace std::literals::chrono_literals;

namespace
{
    class TestClock
    {
    public:
        using rep = int;
        using period = std::milli;
        using duration = std::chrono::duration<rep, period>;
        using time_point = std::chrono::time_point<TestClock>;

        inline static constexpr bool is_steady = true;

        static time_point now() noexcept
        {
            return timeSinceEpoch;
        }

        inline static time_point timeSinceEpoch;
    };

    class TimeTest : public Test
    {
    public:
        MOCK_METHOD(int, callableWithReturnValue, (int), (const));

        MOCK_METHOD(void, callableWithoutReturnValue, (), (const));

    protected:
        Time<TestClock> time;
    };

    TEST_F(TimeTest, InvokeCallableWithReturnValue)
    {
        const int n(42);
        EXPECT_CALL(*this, callableWithReturnValue(n)).WillOnce(Return(n));
        EXPECT_EQ(n, time(&TimeTest::callableWithReturnValue, this, n).first);
    }

    TEST_F(TimeTest, TimeCallableWithReturnValue)
    {
        const auto elapsed = 42ms;
        auto sleep = [&elapsed](int){ TestClock::timeSinceEpoch += elapsed; };

        EXPECT_CALL(*this, callableWithReturnValue).WillOnce(DoAll(Invoke(sleep), Return(0)));
        EXPECT_EQ(elapsed, time(&TimeTest::callableWithReturnValue, this, 0).second);
    }

    TEST_F(TimeTest, TimeCallableWithoutReturnValue)
    {
        const auto elapsed = 313ms;
        auto sleep = [&elapsed]{ TestClock::timeSinceEpoch += elapsed; };

        EXPECT_CALL(*this, callableWithoutReturnValue()).WillOnce(Invoke(sleep));
        EXPECT_EQ(elapsed, time(&TimeTest::callableWithoutReturnValue, this));
    }
}
