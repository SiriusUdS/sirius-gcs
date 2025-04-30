#include "CommandCenter.h"

namespace CommandCenter {
Command commands[Constants::COMMAND_STORAGE_MAX_SIZE];
}

bool CommandCenter::isValidId(size_t commandId) {
    return commandId < Constants::COMMAND_STORAGE_MAX_SIZE;
}

Command* CommandCenter::get(size_t commandId) {
    if (!isValidId(commandId) || commands[commandId].state == CommandState::NOT_READY) {
        return nullptr;
    }
    return &commands[commandId];
}

size_t CommandCenter::reserveSlot() {
    for (size_t i = 0; i < Constants::COMMAND_STORAGE_MAX_SIZE; i++) {
        if (commands[i].state == CommandState::NOT_READY) {
            commands[i].state = CommandState::READY;
            return i;
        }
    }
    return Constants::COMMAND_STORAGE_MAX_SIZE;
}

bool CommandCenter::freeSlot(size_t commandId) {
    if (!isValidId(commandId)) {
        return false;
    }
    commands[commandId].state = CommandState::NOT_READY;
    return true;
}

bool CommandCenter::processAck() {
    return false;
}
