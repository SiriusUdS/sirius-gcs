#ifndef COMMAND_H
#define COMMAND_H

#include "Timer.h"

#include <atomic>
#include <chrono>
#include <stdint.h>

/**
 * @class Command
 * @brief Represents a command that can be sent to the GS.
 */
class Command {
private:
    static constexpr size_t MAX_DATA_SIZE = 256; ///< Maximum size of the command's data
public:
    bool available();
    bool ready(size_t dataSize);
    void process();

    uint8_t data[MAX_DATA_SIZE] = {0}; ///< Command data
private:
    /**
     * @enum State
     * @brief Represents the state of the command.
     */
    enum class State {
        IDLE,    ///< Command is not being sent
        SENDING, ///< Command is being sent (potentially multiple times)
    };

    std::atomic<State> state = State::IDLE; ///< Current state of the command
    size_t size{};                          ///< Size of the command's data
    size_t timesSent{};                     ///< Number of times the same command has been sent
    Timer lastTimeSentTimer;                ///< Timer for the last time the command was sent
};

#endif // COMMAND_H
