#include "SerialFailureMonitor.h"

/**
 * @brief Tracks the success or failure of a read operation.
 */
void SerialFailureMonitor::trackRead(bool successful) {
    if (successful) {
        consecutiveReadsFailed = 0;
    } else {
        consecutiveReadsFailed++;
    }
}

/**
 * @brief Tracks the success or failure of a write operation.
 */
void SerialFailureMonitor::trackWrite(bool successful) {
    if (successful) {
        consecutiveWritesFailed = 0;
    } else {
        consecutiveWritesFailed++;
    }
}

/**
 * @brief Checks if the serial communication is failing based on the number of consecutive failed reads or writes.
 */
bool SerialFailureMonitor::isComFailing() {
    return consecutiveReadsFailed >= CONSECUTIVE_FAILED_READS_BEFORE_FAILURE || consecutiveWritesFailed >= CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE;
}

/**
 * @brief Resets the counters for consecutive failed reads and writes.
 */
void SerialFailureMonitor::reset() {
    consecutiveReadsFailed = 0;
    consecutiveWritesFailed = 0;
}
