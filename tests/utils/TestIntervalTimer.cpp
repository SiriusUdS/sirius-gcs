#include "IntervalTimer.h"

#include <doctest.h>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

TEST_CASE("IntervalTimer should have correct elapsed count") {
    IntervalTimer timer(milliseconds(100));

    CHECK(timer.getElapsedCount() == 0);
    sleep_for(milliseconds(250));
    CHECK(timer.getElapsedCount() == 2);
    sleep_for(milliseconds(150));
    CHECK(timer.getElapsedCount() == 4);
    sleep_for(milliseconds(100));
    CHECK(timer.getElapsedCount() == 5);
}

TEST_CASE("IntervalTimer should reset elapsed count") {
    IntervalTimer timer(milliseconds(200));

    CHECK(timer.getElapsedCount() == 0);
    sleep_for(milliseconds(200));
    CHECK(timer.getElapsedCount() == 1);
    timer.resetElapsedCount();
    CHECK(timer.getElapsedCount() == 0);
    sleep_for(milliseconds(200));
    CHECK(timer.getElapsedCount() == 1);
}
