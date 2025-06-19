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
    /**
     * @enum State
     * @brief Represents the state of the command.
     */
    enum class State {
        NONE,  ///< No state, command not initialized
        READY, ///< Command is ready to be sent
        SENT,  ///< Command has been sent, waiting for acknowledgment
    };
    static constexpr size_t MAX_DATA_SIZE = 256; ///< Maximum size of the command's data

    std::atomic<State> state = State::NONE;             ///< Current state of the command
    uint8_t data[MAX_DATA_SIZE] = {0};                  ///< Command data
    size_t size{};                                      ///< Size of the command's data
    std::chrono::steady_clock::time_point lastTimeSent; ///< Last time the command was sent (used for ACK)
};

#endif // COMMAND_H
