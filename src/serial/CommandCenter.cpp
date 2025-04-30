#include "CommandCenter.h"

namespace CommandCenter {
Command commands[Constants::COMMAND_STORAGE_MAX_SIZE];
}

bool CommandCenter::isValidId(size_t commandId) {
    return commandId < Constants::COMMAND_STORAGE_MAX_SIZE;
}

size_t CommandCenter::reserveSlot() {
    for (size_t i = 0; i < Constants::COMMAND_STORAGE_MAX_SIZE; i++) {
        if (!commands[i].valid) {
            commands[i].valid = true;
            commands[i].sent = false;
            return i;
        }
    }
    return Constants::COMMAND_STORAGE_MAX_SIZE;
}

bool CommandCenter::freeSlot(size_t commandId) {
    if (!isValidId(commandId)) {
        return false;
    }

    commands[commandId].valid = false;
    return true;
}

Command* CommandCenter::get(size_t commandId) {
    if (!isValidId(commandId)) {
        return nullptr;
    }
    return &commands[commandId];
}
