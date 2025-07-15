#include "BoardComStateMonitor.h"

#include <doctest.h>
#include <thread>

using namespace std::this_thread;

void sleep_sec(size_t sec) {
    sleep_for(std::chrono::seconds(sec));
}

TEST_CASE("BoardComStateMonitor state should be \"STARTING\"") {
    BoardComStateMonitor monitor;

    SUBCASE("Initial state") {
        CHECK(monitor.getState() == BoardComStateMonitor::State::STARTING);
    }

    SUBCASE("Following a reset") {
        monitor.reset();
        CHECK(monitor.getState() == BoardComStateMonitor::State::STARTING);
    }

    SUBCASE("No packet received for some time but failure timeout not reached") {
        sleep_sec(BoardComStateMonitor::TIME_WITHOUT_INITIAL_PACKET_READ_BEFORE_FAILURE_SEC - 1);
        CHECK(monitor.getState() == BoardComStateMonitor::State::STARTING);
    }
}

TEST_CASE("SerialStateMonitor state should be \"WORKING\"") {
    BoardComStateMonitor monitor;

    SUBCASE("After a packet was tracked") {
        monitor.trackSuccessfulPacketRead();
        CHECK(monitor.getState() == BoardComStateMonitor::State::WORKING);
    }

    SUBCASE("After a packet was tracked and no other packet was received for some time, but failure timeout not reached") {
        monitor.trackSuccessfulPacketRead();
        sleep_sec(BoardComStateMonitor::TIME_WITHOUT_SUCCESSFUL_PACKET_READ_BEFORE_FAILURE_SEC - 1);
        CHECK(monitor.getState() == BoardComStateMonitor::State::WORKING);
    }
}

TEST_CASE("SerialStateMonitor state should be \"NOT_WORKING\"") {
    BoardComStateMonitor monitor;

    SUBCASE("Com just started and no packet was received for too long") {
        sleep_sec(BoardComStateMonitor::TIME_WITHOUT_INITIAL_PACKET_READ_BEFORE_FAILURE_SEC);
        CHECK(monitor.getState() == BoardComStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("Com just reset and no packet was received for too long") {
        monitor.reset();
        sleep_sec(BoardComStateMonitor::TIME_WITHOUT_INITIAL_PACKET_READ_BEFORE_FAILURE_SEC);
        CHECK(monitor.getState() == BoardComStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("Packet received, then no packet was received for too long") {
        monitor.trackSuccessfulPacketRead();
        sleep_sec(BoardComStateMonitor::TIME_WITHOUT_SUCCESSFUL_PACKET_READ_BEFORE_FAILURE_SEC);
        CHECK(monitor.getState() == BoardComStateMonitor::State::NOT_WORKING);
    }
}
