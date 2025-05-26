#include "CommandCenter.h"

#include "Logging.h"
#include "SerialTask.h"

namespace CommandCenter {
Command command;
std::mutex mtx;
} // namespace CommandCenter

Command& CommandCenter::get() {
    return command;
}

bool CommandCenter::available() {
    return command.state == CommandState::NONE;
}

bool CommandCenter::ready(size_t commandSize) {
    if (command.state != CommandState::NONE) {
        GCS_LOG_WARN("CommandCenter: Couldn't mark command as ready, another command is already being processed.");
        return false;
    }
    command.size = commandSize;
    command.state = CommandState::READY;
    return true;
}

bool CommandCenter::processAck() {
    // TODO: Write ack function
    return true;
}

void CommandCenter::processCommand() {
    if (command.state == CommandState::READY) {
        if (SerialTask::com.write(command.data, command.size)) {
            command.state = CommandState::SENT;
            command.lastTimeSent = std::chrono::steady_clock::now();
        } else {
            GCS_LOG_ERROR("CommandCenter: Couldn't send command over serial communication.");
        }
    } else if (command.state == CommandState::SENT) {
        command.state = CommandState::NONE; // TODO: Switch this back to sent later so ack can be processed
        // auto now = std::chrono::steady_clock::now();
        // auto elapsedTimeLastSentMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - commands[i].lastTimeSent);
        // if (elapsedTimeLastSentMs.count() >= Constants::COMMAND_TIME_BEFORE_RESENDING_MS) {
        //     SerialTask::com.write(commands[i].data, commands[i].size);
        //     commands[i].lastTimeSent = now;
        // }
    }
}
