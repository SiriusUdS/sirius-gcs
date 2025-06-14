#ifndef PACKETRATEMONITOR_H
#define PACKETRATEMONITOR_H

#include <chrono>
#include <mutex>

class PacketRateMonitor {
public:
    void trackPacket();
    double getRatePerSecond();
    void reset();

private:
    static constexpr size_t TIME_POINT_ARR_SIZE = 10;

    std::chrono::steady_clock::time_point timePoints[TIME_POINT_ARR_SIZE];
    bool timePointBufferFilled{};
    size_t timePointIndex{};
    std::mutex mtx;
};

#endif // PACKETRATEMONITOR_H
