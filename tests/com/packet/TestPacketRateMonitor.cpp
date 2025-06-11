#include "PacketRateMonitor.h"

#include <doctest.h>

constexpr double PRM_EPSILON = 1;

void simulatePacketCom(PacketRateMonitor& prm, size_t numPackets, double durationSec) {
    const double TIME_BETWEEN_PACKETS_SEC = durationSec / numPackets;
    auto startTime = std::chrono::steady_clock::now();

    for (size_t packetIdx = 0; packetIdx < numPackets; packetIdx++) {
        auto idealNextTime = startTime + std::chrono::duration<double>(TIME_BETWEEN_PACKETS_SEC * (packetIdx + 1));

        prm.trackPacket();

        auto now = std::chrono::steady_clock::now();
        auto sleepDuration = idealNextTime - now;

        while (sleepDuration > std::chrono::duration<double>::zero()) {
            now = std::chrono::steady_clock::now();
            sleepDuration = idealNextTime - now;
        }
    }
}

TEST_CASE("Get packet rate with packet rate monitor") {
    PacketRateMonitor prm;

    simulatePacketCom(prm, 15, 0.5);
    CHECK(doctest::Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 30);
    simulatePacketCom(prm, 100, 0.2);
    CHECK(doctest::Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 500);

    prm.reset();
    simulatePacketCom(prm, 2, 0.1);
    CHECK(doctest::Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 20);

    prm.reset();
    simulatePacketCom(prm, 5, 0.4);
    CHECK(doctest::Approx(prm.getRatePerSecond()).epsilon(PRM_EPSILON) == 12);
}
