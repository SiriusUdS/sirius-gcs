#ifndef COMMAND_H
#define COMMAND_H

#include "Constants.h"

#include <chrono>
#include <stdint.h>

enum class CommandState { NONE, READY, SENT };

struct Command {
    CommandState state = CommandState::NONE;
    uint8_t data[Constants::COMMAND_MAX_SIZE];
    size_t size{};
    std::chrono::steady_clock::time_point lastTimeSent;
};

#endif // COMMAND_H
