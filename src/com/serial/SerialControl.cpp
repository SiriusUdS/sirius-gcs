#include "SerialControl.h"

#include "SerialCom.h"
#include "SerialStateMonitor.h"
#include "SerialTask.h"
#include "Timer.h"

using namespace std::chrono;

namespace SerialControl {
constexpr size_t CONNECTION_ATTEMPT_DELAY_IN_SECS = 3;

Timer timerConnectionAttempt{seconds(CONNECTION_ATTEMPT_DELAY_IN_SECS)};
Timer timerSerialRead;
} // namespace SerialControl

void SerialControl::startComIfNeeded() {
    if (SerialTask::com.comOpened() && SerialTask::serialStateMonitor.getState() != SerialStateMonitor::State::NOT_WORKING) {
        return;
    }

    if (timerConnectionAttempt.hasElapsed()) {
        timerConnectionAttempt.reset();
        SerialTask::com.start();
    }
}

void SerialControl::readIncomingBytesAtSetRate() {
    constexpr size_t BYTES_TO_READ_PER_SECOND = 19'200;

    double elapsedSeconds = timerSerialRead.getElapsedTimeInSeconds();
    timerSerialRead.reset();

    size_t bytesToRead = (size_t) (BYTES_TO_READ_PER_SECOND * elapsedSeconds);
    while (bytesToRead--) {
        if (!SerialTask::com.read()) {
            break;
        }
    }
}
