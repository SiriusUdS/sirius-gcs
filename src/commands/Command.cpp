#include "Command.h"

#include "Logging.h"
#include "SerialCom.h"
#include "SerialTask.h"

/**
 * @brief Determines if the command is available to be initialised.
 * @returns True if the command is available, false otherwise.
 */
bool Command::available() {
    return state == Command::State::IDLE && SerialTask::com.comOpened();
}

/**
 * @brief Mark the command as ready to be sent.
 * @returns True if the command was successfully marked as ready, false otherwise.
 */
bool Command::ready(size_t dataSize) {
    if (state != Command::State::IDLE) {
        GCS_LOG_WARN("Command: Couldn't mark command as ready, another command is already being processed.");
        return false;
    }
    size = dataSize;
    state = Command::State::SENDING;
    lastTimeSentTimer.reset();
    return true;
}

/**
 * @brief Process the command based on its current state.
 */
void Command::process() {
    static constexpr size_t NUMBER_OF_TIMES_TO_SEND_SAME_COMMAND = 10;
    static constexpr double TIME_BETWEEN_COMMAND_SENDS_SEC = 0.1;

    switch (state) {
    case Command::State::SENDING:
        if (lastTimeSentTimer.getElapsedTimeInSeconds() < TIME_BETWEEN_COMMAND_SENDS_SEC) {
            break;
        }
        if (!SerialTask::com.write(data, size)) {
            GCS_LOG_ERROR("Command: Couldn't send command over serial communication.");
            break;
        }
        timesSent++;
        if (NUMBER_OF_TIMES_TO_SEND_SAME_COMMAND <= timesSent) {
            state = Command::State::IDLE;
        }
        break;
    }
}
