#ifndef PACKETRATEMONITOR_H
#define PACKETRATEMONITOR_H

#include "Constants.h"

#include <chrono>
#include <mutex>

class PacketRateMonitor {
public:
    void trackPacket();
    double getRatePerSecond();
    void reset();

private:
    std::chrono::steady_clock::time_point timePoints[Constants::PACKET_RATE_MONITOR_SAMPLE_TIME_POINT_SIZE];
    bool timePointBufferFilled{};
    size_t timePointIndex{};
    std::mutex mtx;
};

#endif // PACKETRATEMONITOR_H
