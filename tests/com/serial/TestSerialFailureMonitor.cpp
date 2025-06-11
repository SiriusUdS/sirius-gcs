#include "Constants.h"
#include "SerialFailureMonitor.h"

#include <doctest.h>

void consecutiveFailedReads(SerialFailureMonitor& monitor, size_t num) {
    for (size_t i = 0; i < num; i++) {
        monitor.trackRead(false);
    }
}

void consecutiveFailedWrites(SerialFailureMonitor& monitor, size_t num) {
    for (size_t i = 0; i < num; i++) {
        monitor.trackWrite(false);
    }
}

TEST_CASE("SerialFailureMonitor should detect com is working if not enough consecutive failed reads and writes") {
    SerialFailureMonitor monitor;

    consecutiveFailedReads(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE - 1);
    consecutiveFailedWrites(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE - 1);
    CHECK(monitor.isComWorking() == true);
}

TEST_CASE("SerialFailureMonitor consecutive failed reads should be reset by a successful read") {
    SerialFailureMonitor monitor;

    consecutiveFailedReads(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    monitor.trackRead(true);
    CHECK(monitor.isComWorking() == true);
}

TEST_CASE("SerialFailureMonitor consecutive failed writes should be reset by a successful write") {
    SerialFailureMonitor monitor;

    consecutiveFailedWrites(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    monitor.trackWrite(true);
    CHECK(monitor.isComWorking() == true);
}

TEST_CASE("SerialFailureMonitor should detect com is not working after consecutive failed reads") {
    SerialFailureMonitor monitor;

    consecutiveFailedReads(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    CHECK(monitor.isComWorking() == false);
}

TEST_CASE("SerialFailureMonitor should detect com is not working after consecutive failed writes") {
    SerialFailureMonitor monitor;

    consecutiveFailedWrites(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    CHECK(monitor.isComWorking() == false);
}

TEST_CASE("SerialFailureMonitor should detect com is not working after a lot of consecutive failed reads") {
    SerialFailureMonitor monitor;

    consecutiveFailedReads(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE * 10);
    CHECK(monitor.isComWorking() == false);
}

TEST_CASE("SerialFailureMonitor should detect com is not working after a lot of consecutive failed writes") {
    SerialFailureMonitor monitor;

    consecutiveFailedWrites(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE * 10);
    CHECK(monitor.isComWorking() == false);
}

TEST_CASE("SerialFailureMonitor should reset correctly") {
    SerialFailureMonitor monitor;

    consecutiveFailedReads(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    consecutiveFailedWrites(monitor, Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    monitor.reset();
    CHECK(monitor.isComWorking() == true);
    CHECK(monitor.isComWorking() == true);
}
