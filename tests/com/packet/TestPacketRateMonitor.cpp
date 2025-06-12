#include "IntervalTimer.h"
#include "PacketRateMonitor.h"

#include <doctest.h>

using namespace doctest;
using namespace std::chrono;

constexpr double PRM_EPSILON = 1;

void simulatePacketCom(PacketRateMonitor& prm, size_t numPackets, double durationSec) {
    const double TIME_BETWEEN_PACKETS_SEC = durationSec / numPackets;
    IntervalTimer intervalTimer(duration_cast<steady_clock::duration>(duration<double>(TIME_BETWEEN_PACKETS_SEC)));

    for (size_t packetIdx = 0; packetIdx < numPackets; packetIdx++) {
        prm.trackPacket();
        while (intervalTimer.getElapsedCount()) {
            // do nothing
        }
        intervalTimer.resetElapsedCount();
    }
}

TEST_CASE("Get packet rate with packet rate monitor") {
    PacketRateMonitor prm;

    simulatePacketCom(prm, 15, 0.5);
    CHECK(Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 30);
    simulatePacketCom(prm, 100, 0.2);
    CHECK(Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 500);

    prm.reset();
    simulatePacketCom(prm, 2, 0.1);
    CHECK(Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 20);

    prm.reset();
    simulatePacketCom(prm, 5, 0.4);
    CHECK(Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 12);
}
