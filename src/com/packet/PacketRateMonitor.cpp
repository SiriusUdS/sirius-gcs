#include "PacketRateMonitor.h"

void PacketRateMonitor::trackPacket() {
    std::lock_guard lock(mtx);

    timePoints[timePointIndex] = std::chrono::steady_clock::now();
    timePointIndex = (timePointIndex + 1) % Constants::PACKET_RATE_MONITOR_SAMPLE_TIME_POINT_SIZE;
    if (timePointIndex == Constants::PACKET_RATE_MONITOR_SAMPLE_TIME_POINT_SIZE - 1) {
        timePointBufferFilled = true;
    }
}

double PacketRateMonitor::getRatePerSecond() {
    std::lock_guard lock(mtx);

    const size_t count = timePointBufferFilled ? Constants::PACKET_RATE_MONITOR_SAMPLE_TIME_POINT_SIZE : timePointIndex;
    if (count < 2) {
        return 0.0;
    }
    const size_t oldestIndex = timePointBufferFilled ? timePointIndex : 0;
    const size_t newestIndex = timePointBufferFilled ? (timePointIndex + count - 1) % count : timePointIndex - 1;
    const auto duration = std::chrono::duration<double>(timePoints[newestIndex] - timePoints[oldestIndex]).count();
    if (duration <= 0.0) {
        return 0.0;
    }
    return (count - 1) / duration;
}

void PacketRateMonitor::reset() {
    std::lock_guard lock(mtx);

    timePointBufferFilled = false;
    timePointIndex = 0;
}
