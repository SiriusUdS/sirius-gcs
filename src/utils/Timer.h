#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    Timer(std::chrono::steady_clock::duration duration);
    double getElapsedTimeInSeconds() const;
    bool hasElapsed() const;
    void reset();

private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::duration duration{};
};

#endif // TIMER_H
