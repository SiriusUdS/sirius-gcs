#include "SerialControl.h"

#include "SerialCom.h"
#include "SerialFailureMonitor.h"
#include "SerialTask.h"

using namespace std::chrono;

namespace SerialControl {
steady_clock::time_point lastSerialConnectionAttempt = steady_clock::now();
steady_clock::time_point lastSerialReadTime = steady_clock::now();
} // namespace SerialControl

void SerialControl::startComIfNeeded() {
    constexpr size_t CONNECTION_ATTEMPT_DELAY_IN_SECS = 3;

    if (SerialTask::com.comOpened() && SerialTask::serialFailureMonitor.isComWorking()) {
        return;
    }

    auto now = steady_clock::now();
    duration<double> elapsedSeconds = now - lastSerialConnectionAttempt;

    if (elapsedSeconds.count() > CONNECTION_ATTEMPT_DELAY_IN_SECS) {
        lastSerialConnectionAttempt = now;
        SerialTask::com.start();
    }
}

void SerialControl::readIncomingBytesAtSetRate() {
    constexpr size_t BYTES_TO_READ_PER_SECOND = 19'200;

    auto now = steady_clock::now();
    duration<double> elapsedSeconds = now - lastSerialReadTime;
    lastSerialReadTime = now;

    size_t bytesToRead = (size_t) (BYTES_TO_READ_PER_SECOND * elapsedSeconds.count());
    while (0 < bytesToRead--) {
        if (!SerialTask::com.read()) {
            break;
        }
    }
}
