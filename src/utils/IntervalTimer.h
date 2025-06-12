#ifndef INTERVALTIMER_H
#define INTERVALTIMER_H

#include <chrono>

class IntervalTimer {
public:
    IntervalTimer(std::chrono::steady_clock::duration interval);
    size_t getElapsedCount() const;
    void resetElapsedCount();

private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::duration interval;
};

#endif // INTERVALTIMER_H
