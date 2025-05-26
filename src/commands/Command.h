#ifndef COMMAND_H
#define COMMAND_H

#include "Constants.h"

#include <atomic>
#include <chrono>
#include <stdint.h>

enum class CommandState { NONE, READY, SENT };

/**
 * @struct Command
 * @brief Represents a command that can be sent to the GS.
 */
struct Command {
    std::atomic<CommandState> state = CommandState::NONE; ///< Current state of the command
    uint8_t data[Constants::COMMAND_MAX_SIZE];            ///< Command data
    size_t size{};                                        ///< Size of the command's data
    std::chrono::steady_clock::time_point lastTimeSent;   ///< Last time the command was sent (used for ACK)
};

#endif // COMMAND_H
