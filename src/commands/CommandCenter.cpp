#include "CommandCenter.h"

#include "Logging.h"

namespace CommandCenter {
Command commands[Constants::COMMAND_STORAGE_MAX_SIZE];
}

Command* CommandCenter::get(size_t commandId) {
    if (!isValidId(commandId) || commands[commandId].state == CommandState::NONE) {
        GCS_LOG_WARN("CommandCenter: Command ID {} is invalid.", commandId);
        return nullptr;
    }
    return &commands[commandId];
}

size_t CommandCenter::reserveSlot() {
    for (size_t i = 0; i < Constants::COMMAND_STORAGE_MAX_SIZE; i++) {
        if (commands[i].state == CommandState::NONE) {
            commands[i].state = CommandState::READY;
            return i;
        }
    }
    return Constants::COMMAND_STORAGE_MAX_SIZE;
}

bool CommandCenter::freeSlot(size_t commandId) {
    if (!isValidId(commandId)) {
        GCS_LOG_WARN("CommandCenter: Command ID {} is invalid.", commandId);
        return false;
    }
    commands[commandId].state = CommandState::NONE;
    return true;
}

bool CommandCenter::processAck(size_t commandId) {
    if (!isValidId(commandId)) {
        GCS_LOG_WARN("CommandCenter: Command ID {} is invalid.", commandId);
        return false;
    }
    if (commands[commandId].state == CommandState::NONE) {
        GCS_LOG_WARN("CommandCenter: Tried to process ACK packet for command that doesn't exist.");
        return false;
    } else if (commands[commandId].state == CommandState::READY) {
        GCS_LOG_WARN("CommandCenter: Tried to process ACK packet for command that hasn't been sent yet.");
        return false;
    }
    commands[commandId].state = CommandState::NONE;
    return true;
}

bool CommandCenter::isValidId(size_t commandId) {
    return commandId < Constants::COMMAND_STORAGE_MAX_SIZE;
}
