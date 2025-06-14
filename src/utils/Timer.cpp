#include "Timer.h"

using namespace std::chrono;

Timer::Timer() : startTime(steady_clock::now()) {
}

Timer::Timer(steady_clock::duration duration) : startTime(steady_clock::now()), duration(duration) {
}

double Timer::getElapsedTimeInSeconds() const {
    return std::chrono::duration<double>(steady_clock::now() - startTime).count();
}

bool Timer::hasElapsed() const {
    return (steady_clock::now() - startTime) >= duration;
}

void Timer::reset() {
    startTime = steady_clock::now();
}
