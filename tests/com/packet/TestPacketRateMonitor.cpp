#include "IntervalTimer.h"
#include "PacketRateMonitor.h"

#include <doctest.h>
#include <thread>

using namespace doctest;
using namespace std::chrono;
using namespace std::this_thread;

constexpr double PRM_EPSILON = 0.05;

double simulatePacketCom(PacketRateMonitor& prm, size_t numPackets, double durationSec) {
    const double TIME_BETWEEN_PACKETS_SEC = durationSec / numPackets;
    IntervalTimer intervalTimer(duration_cast<steady_clock::duration>(duration<double>(TIME_BETWEEN_PACKETS_SEC)));

    for (size_t packetIdx = 0; packetIdx < numPackets; packetIdx++) {
        prm.trackPacket();
        while (!intervalTimer.getElapsedCount()) {
            // do nothing
        }
        intervalTimer.resetElapsedCount();
    }

    return prm.getRatePerSecond();
}

TEST_CASE("PacketRateMonitor should return correct rate") {
    PacketRateMonitor prm;
    double elapsedSec;

    elapsedSec = simulatePacketCom(prm, 15, 0.5);
    CHECK(Approx(elapsedSec).epsilon(PRM_EPSILON) == 30);

    prm.reset();
    elapsedSec = simulatePacketCom(prm, 250, 0.5);
    CHECK(Approx(elapsedSec).epsilon(PRM_EPSILON) == 500);

    prm.reset();
    elapsedSec = simulatePacketCom(prm, 2, 0.1);
    CHECK(Approx(elapsedSec).epsilon(PRM_EPSILON) == 20);

    prm.reset();
    elapsedSec = simulatePacketCom(prm, 5, 0.4);
    CHECK(Approx(elapsedSec).epsilon(PRM_EPSILON) == 12.5);
}

TEST_CASE("PacketRateMonitor rate should go back to 0 after waiting long enough after receiving packets") {
    PacketRateMonitor prm;

    simulatePacketCom(prm, 15, 0.5);
    sleep_for(seconds((size_t) PacketRateMonitor::TIME_WINDOW_SECONDS));
    CHECK(prm.getRatePerSecond() == 0);
}
