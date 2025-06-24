#include "IntervalTimer.h"

#include <thread>

using namespace std::this_thread;
using namespace std::chrono;

/**
 * @brief Constructs an IntervalTimer with a specified interval.
 */
IntervalTimer::IntervalTimer(std::chrono::steady_clock::duration interval) : startTime(steady_clock::now()), interval(interval) {
}

/**
 * @brief Waits until the next interval.
 */
void IntervalTimer::waitUntilNextInterval() const {
    sleep_for(getTimeUntilNextInterval());
}

/**
 * @brief Returns the time remaining until the next interval.
 */
std::chrono::steady_clock::duration IntervalTimer::getTimeUntilNextInterval() const {
    const auto now = steady_clock::now();
    const auto elapsed = now - startTime;
    return interval - (elapsed % interval);
}
