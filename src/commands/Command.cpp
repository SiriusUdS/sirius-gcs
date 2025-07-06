#include "Command.h"

#include "Logging.h"
#include "SerialCom.h"
#include "SerialTask.h"

/**
 * @brief Determines if the command is available to be initialised.
 * @returns True if the command is available, false otherwise.
 */
bool Command::available() {
    return state == Command::State::NONE && SerialTask::com.comOpened();
}

/**
 * @brief Mark the command as ready to be sent.
 * @returns True if the command was successfully marked as ready, false otherwise.
 */
bool Command::ready(size_t dataSize) {
    if (state != Command::State::NONE) {
        GCS_LOG_WARN("Command: Couldn't mark command as ready, another command is already being processed.");
        return false;
    }
    size = dataSize;
    state = Command::State::SENDING;
    lastTimeSentTimer.reset();
    return true;
}

/**
 * @brief Process an acknowledgment packet.
 * @returns True if the acknowledgment was successfully processed, false otherwise.
 */
bool Command::ack() {
    // TODO: Write ack function
    return true;
}

/**
 * @brief Process the command based on its current state.
 */
void Command::process() {
    static constexpr size_t NUMBER_OF_TIMES_TO_SEND_SAME_COMMAND = 5;
    static constexpr double TIME_BETWEEN_COMMAND_SENDS_SEC = 0.03;

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
            state = Command::State::SENT;
        }
        break;
    case Command::State::SENT:
        timesSent = 0;
        state = Command::State::NONE; // TODO: Implement ack
        break;
    }
}
