#ifndef COMMAND_H
#define COMMAND_H

#include <atomic>
#include <chrono>
#include <stdint.h>

/**
 * @struct Command
 * @brief Represents a command that can be sent to the GS.
 */
struct Command {
    enum class State { NONE, READY, SENT };
    static constexpr size_t MAX_DATA_SIZE = 256;

    std::atomic<State> state = State::NONE;             ///< Current state of the command
    uint8_t data[MAX_DATA_SIZE] = {0};                  ///< Command data
    size_t size{};                                      ///< Size of the command's data
    std::chrono::steady_clock::time_point lastTimeSent; ///< Last time the command was sent (used for ACK)
};

#endif // COMMAND_H
