#include "Command.h"

#include "Logging.h"
#include "SerialCom.h"
#include "SerialTask.h"

bool Command::available() {
    return state == Command::State::NONE && SerialTask::com.comOpened();
}

bool Command::ready(size_t dataSize) {
    if (state != Command::State::NONE) {
        GCS_LOG_WARN("CommandCenter: Couldn't mark command as ready, another command is already being processed.");
        return false;
    }
    size = dataSize;
    state = Command::State::READY;
    return true;
}

bool Command::ack() {
    // TODO: Write ack function
    return true;
}

void Command::process() {
    if (state == Command::State::READY) {
        if (SerialTask::com.write(data, size)) {
            state = Command::State::SENT;
            lastTimeSent = std::chrono::steady_clock::now();
        } else {
            GCS_LOG_ERROR("CommandCenter: Couldn't send command over serial communication.");
        }
    } else if (state == Command::State::SENT) {
        state = Command::State::NONE; // TODO: Implement ack
    }
}
