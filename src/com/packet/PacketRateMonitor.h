#ifndef PACKETRATEMONITOR_H
#define PACKETRATEMONITOR_H

#include <chrono>
#include <deque>
#include <mutex>

class PacketRateMonitor {
public:
    void trackPacket();
    double getRatePerSecond();
    void reset();

    static constexpr double TIME_WINDOW_SECONDS = 2.0;

private:
    void removeOldTimePoints();

    std::deque<std::chrono::steady_clock::time_point> timePoints;
    std::mutex mtx;
};

#endif // PACKETRATEMONITOR_H
