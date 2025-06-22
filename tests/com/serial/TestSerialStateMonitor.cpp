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

    CHECK(monitor.getState() == SerialStateMonitor::State::STARTING);
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
    CHECK(monitor.getState() == SerialStateMonitor::State::STARTING);
}

TEST_CASE("SerialStateMonitor state should be \"RESETTING\"") {
    SerialStateMonitor monitor;

    monitor.reset();
    CHECK(monitor.getState() == SerialStateMonitor::State::RESETTING);
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
    CHECK(monitor.getState() == SerialStateMonitor::State::RESETTING);
}

TEST_CASE("SerialStateMonitor state should be \"WORKING\"") {
    SerialStateMonitor monitor;

    successfulRead(monitor);
    CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
    CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    monitor.reset();
    successfulWrite(monitor);
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
    CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
    monitor.reset();
    successfulRead(monitor);
    CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
    monitor.reset();
    successfulWrite(monitor);
    CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
}

TEST_CASE("SerialStateMonitor state should be \"NOT_WORKING\"") {
    SerialStateMonitor monitor;
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    successfulRead(monitor);
    CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
    CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    successfulWrite(monitor);
    consecutiveFailedReads(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_READS_BEFORE_FAILURE * 10);
    CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
    successfulRead(monitor);
    CHECK(monitor.getState() == SerialStateMonitor::State::WORKING);
    consecutiveFailedWrites(monitor, SerialStateMonitor::CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE * 10);
    CHECK(monitor.getState() == SerialStateMonitor::State::NOT_WORKING);
}
