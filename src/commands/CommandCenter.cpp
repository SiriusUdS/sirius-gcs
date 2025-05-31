#include "CommandCenter.h"

#include "Command.h"
#include "Logging.h"
#include "SerialCom.h"
#include "SerialTask.h"

namespace CommandCenter {
Command command;
std::mutex mtx;
} // namespace CommandCenter

Command& CommandCenter::get() {
    return command;
}

bool CommandCenter::available() {
    return command.state == CommandState::NONE && SerialTask::com.comOpened();
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
        command.state = CommandState::NONE; // TODO: Implement Ack
    }
}
