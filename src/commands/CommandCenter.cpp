#include "CommandCenter.h"

#include "Logging.h"
#include "SerialTask.h"

namespace CommandCenter {
Command commands[Constants::COMMAND_STORAGE_MAX_SIZE];
std::mutex mtx;
} // namespace CommandCenter

Command* CommandCenter::get(size_t commandSlotId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!isValidSlotId(commandSlotId) || commands[commandSlotId].state == CommandState::NONE) {
        GCS_LOG_WARN("CommandCenter: Command ID {} is invalid or unreserved.", commandSlotId);
        return nullptr;
    }
    return &commands[commandSlotId];
}

size_t CommandCenter::reserveSlot() {
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < Constants::COMMAND_STORAGE_MAX_SIZE; i++) {
        if (commands[i].state == CommandState::NONE) {
            commands[i].state = CommandState::RESERVED;
            return i;
        }
    }
    GCS_LOG_WARN("CommandCenter: Couldn't reserve command slot, no more space available.");
    return Constants::COMMAND_STORAGE_MAX_SIZE;
}

bool CommandCenter::ready(size_t commandSlotId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (commands[commandSlotId].state != CommandState::RESERVED) {
        GCS_LOG_WARN("CommandCenter: Couldn't mark command as ready as the state wasn't RESERVED.");
        return false;
    }
    commands[commandSlotId].state = CommandState::READY;
    return true;
}

bool CommandCenter::freeSlot(size_t commandSlotId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!isValidSlotId(commandSlotId)) {
        GCS_LOG_WARN("CommandCenter: Command ID {} is invalid.", commandSlotId);
        return false;
    }
    commands[commandSlotId].state = CommandState::NONE;
    return true;
}

bool CommandCenter::processAck(size_t commandSlotId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!isValidSlotId(commandSlotId)) {
        GCS_LOG_WARN("CommandCenter: Command ID {} is invalid.", commandSlotId);
        return false;
    } else if (commands[commandSlotId].state == CommandState::NONE) {
        GCS_LOG_WARN("CommandCenter: Tried to process ACK packet for command that doesn't exist.");
        return false;
    } else if (commands[commandSlotId].state == CommandState::READY) {
        GCS_LOG_WARN("CommandCenter: Tried to process ACK packet for command that hasn't been sent yet.");
        return false;
    }
    // commands[commandSlotId].state = CommandState::NONE;
    // freeSlot(commandSlotId);
    return true;
}

void CommandCenter::processCommands() {
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < Constants::COMMAND_STORAGE_MAX_SIZE; i++) {
        if (commands[i].state == CommandState::READY) {
            SerialTask::com.write(commands[i].data, commands[i].size);
            commands[i].state = CommandState::SENT;
            commands[i].lastTimeSent = std::chrono::steady_clock::now();
        } else if (commands[i].state == CommandState::SENT) {
            commands[i].state = CommandState::NONE; // TODO: Switch this back to sent later so ack can be processed
            // auto now = std::chrono::steady_clock::now();
            // auto elapsedTimeLastSentMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - commands[i].lastTimeSent);
            // if (elapsedTimeLastSentMs.count() >= Constants::COMMAND_TIME_BEFORE_RESENDING_MS) {
            //     SerialTask::com.write(commands[i].data, commands[i].size);
            //     commands[i].lastTimeSent = now;
            // }
        }
    }
}

bool CommandCenter::isValidSlotId(size_t commandSlotId) {
    return commandSlotId < Constants::COMMAND_STORAGE_MAX_SIZE;
}
