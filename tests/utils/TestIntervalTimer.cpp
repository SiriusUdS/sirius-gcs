#include "IntervalTimer.h"

#include <doctest.h>
#include <thread>

using namespace doctest;
using namespace std::chrono;
using namespace std::this_thread;

constexpr double TIMER_EPSILON = 0.1;

TEST_CASE("IntervalTimer should wait until next interval") {
    IntervalTimer timer(milliseconds(1000));
    size_t elapsedMs;
    steady_clock::time_point startTime;

    startTime = steady_clock::now();
    timer.waitUntilNextInterval();
    elapsedMs = duration_cast<milliseconds>(steady_clock::now() - startTime).count();
    CHECK(Approx((double) elapsedMs).epsilon(TIMER_EPSILON) == 1000);

    sleep_for(milliseconds(500));
    startTime = steady_clock::now();
    timer.waitUntilNextInterval();
    elapsedMs = duration_cast<milliseconds>(steady_clock::now() - startTime).count();
    CHECK(Approx((double) elapsedMs).epsilon(TIMER_EPSILON) == 500);

    sleep_for(milliseconds(1750));
    startTime = steady_clock::now();
    timer.waitUntilNextInterval();
    elapsedMs = duration_cast<milliseconds>(steady_clock::now() - startTime).count();
    CHECK(Approx((double) elapsedMs).epsilon(TIMER_EPSILON) == 250);
}
