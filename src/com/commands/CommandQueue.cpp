#include "CommandQueue.h"

#include "Logging.h"

// Add last-writer-wins implementation explanation
void CommandQueue::enqueue(CommandType type, uint32_t value) {
    size_t idx = static_cast<std::size_t>(type);

    if (!values[idx].has_value()) {
        pendingTypes.push(type);
    }
    values[idx] = value;
}

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

bool CommandQueue::empty() const {
    return pendingTypes.empty();
}
