#include "IntervalTimer.h"

using namespace std::chrono;

/**
 * @brief Constructs an IntervalTimer with a specified interval.
 */
IntervalTimer::IntervalTimer(std::chrono::steady_clock::duration interval) : startTime(steady_clock::now()), interval(interval) {
}

/**
 * @brief Returns the number of elapsed intervals since the timer was started.
 */
size_t IntervalTimer::getElapsedCount() const {
    const auto now = steady_clock::now();
    auto elapsed = now - startTime;
    return elapsed / interval;
}

/**
 * @brief Resets the count of elapsed intervals.
 */
void IntervalTimer::resetElapsedCount() {
    const auto now = steady_clock::now();
    const auto elapsed = now - startTime;
    const auto startTimeAdjust = elapsed - (elapsed % interval);
    startTime += startTimeAdjust;
}
