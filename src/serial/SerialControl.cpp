#include "SerialControl.h"

#include "Application.h"
#include "Constants.h"

namespace SerialControl {
std::chrono::time_point<std::chrono::steady_clock> lastSerialConnectionAttempt = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> lastSerialReadTime = std::chrono::steady_clock::now();
} // namespace SerialControl

void SerialControl::startComIfNeeded() {
    if (Application::serialCom.comOpened() && Application::serialCom.comWorking()) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - lastSerialConnectionAttempt;

    if (elapsedSeconds.count() > Constants::SERIAL_CONNECTION_ATTEMPT_DELAY_IN_SECS) {
        lastSerialConnectionAttempt = now;
        Application::serialCom.start();
    }
}

void SerialControl::readIncomingBytesAtSetRate() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - lastSerialReadTime;
    lastSerialReadTime = now;

    size_t bytesToRead = Constants::RECV_BYTES_TO_READ_PER_SECOND * elapsedSeconds.count();
    while (0 < bytesToRead--) {
        if (!Application::serialCom.read()) {
            break;
        }
    }
}
