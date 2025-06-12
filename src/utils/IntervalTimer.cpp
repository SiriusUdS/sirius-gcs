#include "IntervalTimer.h"

using namespace std::chrono;

IntervalTimer::IntervalTimer(steady_clock::duration interval) : startTime(steady_clock::now()), interval(interval) {
}

size_t IntervalTimer::getElapsedCount() const {
    const auto now = steady_clock::now();
    auto elapsed = now - startTime;
    return elapsed / interval;
}

void IntervalTimer::resetElapsedCount() {
    const auto now = steady_clock::now();
    const auto elapsed = now - startTime;
    const auto startTimeAdjust = elapsed - (elapsed % interval);
    startTime += startTimeAdjust;
}
