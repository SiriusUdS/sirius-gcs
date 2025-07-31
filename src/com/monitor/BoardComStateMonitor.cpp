#include "BoardComStateMonitor.h"

/**
 * @brief Tracks a successful packet read;
 */
void BoardComStateMonitor::trackSuccessfulPacketRead() {
    lastSuccessfulPacketReadTimer.reset();
    ioSuccessSinceStart = true;
}

/**
 * @brief Resets the monitor state.
 */
void BoardComStateMonitor::reset() {
    lastSuccessfulPacketReadTimer.reset();
    ioSuccessSinceStart = false;
}

/**
 * @brief Gets the current state of the board communication.
 * @returns The current state of the board communication.
 */
BoardComStateMonitor::State BoardComStateMonitor::getState() const {
    const double secondsSinceLastPacketRead = lastSuccessfulPacketReadTimer.getElapsedTimeInSeconds();
    if (ioSuccessSinceStart) {
        if (secondsSinceLastPacketRead > TIME_WITHOUT_SUCCESSFUL_PACKET_READ_BEFORE_FAILURE_SEC) {
            return NOT_WORKING;
        } else {
            return WORKING;
        }
    } else {
        if (secondsSinceLastPacketRead > TIME_WITHOUT_INITIAL_PACKET_READ_BEFORE_FAILURE_SEC) {
            return NOT_WORKING;
        } else {
            return STARTING;
        }
    }
}
