#include "Timer.h"

#include <doctest.h>
#include <thread>

using namespace doctest;
using namespace std::chrono;
using namespace std::this_thread;

constexpr double ELAPSED_EPSILON = 0.05;

TEST_CASE("Timer should return elapsed time in seconds") {
    Timer timer;
    sleep_for(milliseconds(100));
    CHECK(Approx(timer.getElapsedTimeInSeconds()).epsilon(ELAPSED_EPSILON) == 0.1);
    sleep_for(milliseconds(400));
    CHECK(Approx(timer.getElapsedTimeInSeconds()).epsilon(ELAPSED_EPSILON) == 0.5);
    sleep_for(milliseconds(50));
    CHECK(Approx(timer.getElapsedTimeInSeconds()).epsilon(ELAPSED_EPSILON) == 0.55);
}

TEST_CASE("Timer should ring") {
    Timer timer(milliseconds(50));
    CHECK_FALSE(timer.hasElapsed());
    sleep_for(milliseconds(50));
    CHECK(timer.hasElapsed());
}

TEST_CASE("Timer should reset") {
    Timer timer(milliseconds(100));
    sleep_for(milliseconds(100));
    CHECK(Approx(timer.getElapsedTimeInSeconds()).epsilon(ELAPSED_EPSILON) == 0.1);
    CHECK(timer.hasElapsed());
    timer.reset();
    CHECK_FALSE(timer.hasElapsed());
    CHECK(Approx(timer.getElapsedTimeInSeconds()).epsilon(ELAPSED_EPSILON) == 0.0);
    sleep_for(milliseconds(100));
    CHECK(Approx(timer.getElapsedTimeInSeconds()).epsilon(ELAPSED_EPSILON) == 0.1);
    CHECK(timer.hasElapsed());
}
