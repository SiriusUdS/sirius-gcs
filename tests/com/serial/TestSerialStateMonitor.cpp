#include "SerialStateMonitor.h"

#include <doctest.h>

void consecutiveFailedReads(SerialStateMonitor& monitor, size_t num) {
    for (size_t i = 0; i < num; i++) {
        monitor.trackRead(false);
    }
}

void consecutiveFailedWrites(SerialStateMonitor& monitor, size_t num) {
    for (size_t i = 0; i < num; i++) {
        monitor.trackWrite(false);
    }
}

void successfulRead(SerialStateMonitor& monitor) {
    monitor.trackRead(true);
}

void successfulWrite(SerialStateMonitor& monitor) {
    monitor.trackWrite(true);
}

TEST_CASE("SerialStateMonitor initial state should be \"STARTING\"") {
    SerialStateMonitor monitor;

    SUBCASE("Initial state") {
        CHECK(monitor.getState() == SerialStateMonitor::State::STARTING);
    }

    SUBCASE("No successful io but not enough consecutive failed io") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
        CHECK(monitor.getState() == SerialStateMonitor::State::STARTING);
    }
}

TEST_CASE("SerialStateMonitor state should be \"RESETTING\"") {
    SerialStateMonitor monitor;
    monitor.reset();

    SUBCASE("After reset") {
        CHECK(monitor.getState() == SerialStateMonitor::State::RESETTING);
    }

    SUBCASE("After reset with no successful io but not enough consecutive failed io") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
        CHECK(monitor.getState() == SerialStateMonitor::State::RESETTING);
    }
}

TEST_CASE("SerialStateMonitor state should be \"WORKING\"") {
    SerialStateMonitor monitor;

    SUBCASE("After successful read") {
        successfulRead(monitor);
        CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    }

    SUBCASE("After successful read and not enough consecutive failed io") {
        successfulRead(monitor);
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
        CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    }

    SUBCASE("After successful write and not enough consecutive failed io") {
        successfulWrite(monitor);
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
        CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    }

    SUBCASE("After too many failed io followed by successful read") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
        successfulRead(monitor);
        CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    }

    SUBCASE("After too many failed io followed by successful write") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
        successfulWrite(monitor);
        CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    }

    SUBCASE("After reset followed by successful read") {
        monitor.reset();
        successfulRead(monitor);
        CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    }

    SUBCASE("After reset followed by successful write") {
        monitor.reset();
        successfulWrite(monitor);
        CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    }
}

TEST_CASE("SerialStateMonitor state should be \"NOT_WORKING\"") {
    SerialStateMonitor monitor;

    SUBCASE("After too many consecutive failed reads") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After too many consecutive failed writes") {
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After too many consecutive failed reads and writes") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After an excessive amount of consecutive failed reads") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE * 10);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After an excessive amount of consecutive failed writes") {
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE * 10);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After an excessive amount of consecutive failed reads and writes") {
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE * 10);
        consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE * 10);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After successful read followed by too many consecutive failed reads") {
        successfulRead(monitor);
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After successful write followed by too many consecutive failed reads") {
        successfulWrite(monitor);
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After successful read followed by too many consecutive failed writes") {
        successfulRead(monitor);
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }

    SUBCASE("After successful write followed by too many consecutive failed writes") {
        successfulWrite(monitor);
        consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
        CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    }
}
