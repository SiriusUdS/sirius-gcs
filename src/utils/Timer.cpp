#include "Timer.h"

using namespace std::chrono;

/**
 * @brief Constructs a timer without setting a specific duration.
 */
Timer::Timer() : startTime(steady_clock::now()) {
}

/**
 * @brief Constructs a timer with a specified duration.
 * @param duration The duration after which the timer is considered elapsed.
 */
Timer::Timer(std::chrono::steady_clock::duration duration) : startTime(steady_clock::now()), duration(duration) {
}

/**
 * @brief Returns the elapsed time in seconds since the timer was started.
 */
double Timer::getElapsedTimeInSeconds() const {
    return std::chrono::duration<double>(steady_clock::now() - startTime).count();
}

/**
 * @brief Checks if the specified duration has elapsed since the timer was started.
 * @return True if the duration has elapsed, false otherwise.
 */
bool Timer::hasElapsed() const {
    return (steady_clock::now() - startTime) >= duration;
}

/**
 * @brief Resets the timer to the current time.
 */
void Timer::reset() {
    startTime = steady_clock::now();
}
