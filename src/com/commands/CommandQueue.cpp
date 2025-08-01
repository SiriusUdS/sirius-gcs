#include "CommandQueue.h"

#include "Logging.h"

/**
 * @brief Queues commands for later processing.
 * @param type The type of command to enqueue.
 * @param value The value associated with the command.
 */
void CommandQueue::enqueue(CommandType type, uint32_t value) {
    size_t idx = static_cast<std::size_t>(type);

    if (!values[idx].has_value()) {
        pendingTypes.push(type);
    }
    values[idx] = value;
}

/**
 * @brief Dequeues the next command from the queue.
 */
std::optional<Command> CommandQueue::dequeue() {
    if (pendingTypes.empty()) {
        return std::nullopt;
    }

    CommandType type = pendingTypes.front();
    pendingTypes.pop();

    size_t idx = static_cast<std::size_t>(type);

    if (!values[idx].has_value()) {
        GCS_APP_LOG_ERROR("CommandQueue: Internal desync, no value for CommandType {}.", static_cast<int>(type));
        return std::nullopt;
    }

    uint32_t value = *values[idx];
    values[idx] = std::nullopt;

    return Command{.type = type, .value = value};
}

/**
 * @brief Checks if the command queue is empty.
 * @returns True if the queue is empty, false otherwise.
 */
bool CommandQueue::empty() const {
    return pendingTypes.empty();
}
