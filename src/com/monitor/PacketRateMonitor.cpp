#include "PacketRateMonitor.h"

using namespace std::chrono;

/**
 * @brief Tracks an incoming packet to compute the rate of packets.
 */
void PacketRateMonitor::trackPacket() {
    std::lock_guard lock(mtx);

    removeOldTimePoints();
    auto now = steady_clock::now();
    timePoints.push_back(now);
}

/**
 * @brief Computes the rate of packets per second over the defined time window.
 * @return The rate of packets per second.
 */
double PacketRateMonitor::getRatePerSecond() {
    std::lock_guard lock(mtx);

    removeOldTimePoints();
    auto now = steady_clock::now();
    return timePoints.size() / TIME_WINDOW_SECONDS;
}

/**
 * @brief Resets the monitor by clearing all tracked time points.
 */
void PacketRateMonitor::reset() {
    std::lock_guard lock(mtx);

    timePoints.clear();
}

/**
 * @brief Removes time points that are older than the defined time window.
 */
void PacketRateMonitor::removeOldTimePoints() {
    while (!timePoints.empty() && duration_cast<duration<double>>(steady_clock::now() - timePoints.front()).count() > TIME_WINDOW_SECONDS) {
        timePoints.pop_front();
    }
}
