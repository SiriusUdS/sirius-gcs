#include "PacketRateMonitor.h"

using namespace std::chrono;

void PacketRateMonitor::trackPacket() {
    std::lock_guard lock(mtx);
    auto now = steady_clock::now();
    timePoints.push_back(now);

    removeOldTimePoints();
}

double PacketRateMonitor::getRatePerSecond() {
    std::lock_guard lock(mtx);

    auto now = steady_clock::now();

    removeOldTimePoints();

    if (timePoints.empty()) {
        return 0.0;
    }

    double durationSec = duration_cast<duration<double>>(now - timePoints.front()).count();
    return timePoints.size() / durationSec;
}

void PacketRateMonitor::reset() {
    std::lock_guard lock(mtx);
    timePoints.clear();
}

void PacketRateMonitor::removeOldTimePoints() {
    while (!timePoints.empty() && duration_cast<duration<double>>(steady_clock::now() - timePoints.front()).count() > TIME_WINDOW_SECONDS) {
        timePoints.pop_front();
    }
}
