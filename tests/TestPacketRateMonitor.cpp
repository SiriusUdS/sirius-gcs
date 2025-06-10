#include "PacketRateMonitor.h"

#include <doctest.h>

constexpr double PRM_EPSILON = 0.01;

void simulatePacketCom(PacketRateMonitor& prm, size_t numPackets, double durationSec) {
    const double TIME_BETWEEN_PACKETS_SEC = durationSec / numPackets;

    for (size_t packetIdx = 0; packetIdx < numPackets; packetIdx++) {
        std::this_thread::sleep_for(std::chrono::duration<double>(TIME_BETWEEN_PACKETS_SEC));
        prm.trackPacket();
    }
}

TEST_CASE("Get packet rate with packet rate monitor") {
    PacketRateMonitor prm;

    simulatePacketCom(prm, 15, 0.5);
    CHECK(doctest::Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 30);
    simulatePacketCom(prm, 1000, 0.2);
    CHECK(doctest::Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 10000);
}
