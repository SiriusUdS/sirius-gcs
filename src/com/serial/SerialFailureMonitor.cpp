#include "SerialFailureMonitor.h"

void SerialFailureMonitor::trackRead(bool successful) {
    if (successful) {
        consecutiveReadsFailed = 0;
    } else {
        consecutiveReadsFailed++;
    }
}

void SerialFailureMonitor::trackWrite(bool successful) {
    if (successful) {
        consecutiveWritesFailed = 0;
    } else {
        consecutiveWritesFailed++;
    }
}

bool SerialFailureMonitor::isComFailing() {
    return consecutiveReadsFailed >= CONSECUTIVE_FAILED_READS_BEFORE_FAILURE || consecutiveWritesFailed >= CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE;
}

void SerialFailureMonitor::reset() {
    consecutiveReadsFailed = 0;
    consecutiveWritesFailed = 0;
}
