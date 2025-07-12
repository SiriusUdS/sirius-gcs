#include "SerialStateMonitor.h"

#include <algorithm>

/**
 * @brief Tracks the success or failure of a read operation.
 */
void SerialStateMonitor::trackRead(bool successful) {
    if (successful) {
        consecutiveReadsFailed = 0;
        consecutiveWritesFailed = 0;
        ioSuccessSinceStart = true;
    } else {
        consecutiveReadsFailed = std::min(consecutiveReadsFailed + 1, CONSECUTIVE_FAILED_READS_BEFORE_FAILURE);
    }
}

/**
 * @brief Tracks the success or failure of a write operation.
 */
void SerialStateMonitor::trackWrite(bool successful) {
    if (successful) {
        consecutiveReadsFailed = 0;
        consecutiveWritesFailed = 0;
        ioSuccessSinceStart = true;
    } else {
        consecutiveWritesFailed = std::min(consecutiveWritesFailed + 1, CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE);
    }
}

/**
 * @brief Tracks successful packet reads.
 */
void SerialStateMonitor::trackSuccessfulPacketRead() {
    lastSuccessfulPacketReadTimer.reset();
}

/**
 * @brief Resets the counters for consecutive failed reads and writes.
 */
void SerialStateMonitor::reset() {
    ioSuccessSinceStart = false;
    consecutiveReadsFailed = 0;
    consecutiveWritesFailed = 0;
    lastSuccessfulPacketReadTimer.reset();
}

/**
 * @brief Checks if the serial communication is failing based on the number of consecutive failed reads or writes.
 */
SerialStateMonitor::State SerialStateMonitor::getState() const {
    const double secondsSinceLastPacketRead = lastSuccessfulPacketReadTimer.getElapsedTimeInSeconds();
    const bool tooLongSinceLastPacketRead = ioSuccessSinceStart ? secondsSinceLastPacketRead > TIME_WITHOUT_SUCCESSFUL_PACKET_READ_BEFORE_FAILURE_SEC
                                                                : secondsSinceLastPacketRead > TIME_WITHOUT_INITIAL_PACKET_READ_BEFORE_FAILURE_SEC;
    if (consecutiveReadsFailed >= CONSECUTIVE_FAILED_READS_BEFORE_FAILURE || consecutiveWritesFailed >= CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE
        || tooLongSinceLastPacketRead) {
        return State::NOT_WORKING;
    } else if (!ioSuccessSinceStart) {
        return State::STARTING;
    } else {
        return State::WORKING;
    }
}
