#include "CommandQueue.h"

bool CommandQueue::enqueue(CommandType type, uint32_t value) {
    auto idx = static_cast<std::size_t>(type);
    if (inUse[idx]) {
        return false;
    }

    queue.push({type, value});
    inUse[idx] = true;
    return true;
}

std::optional<CommandData> CommandQueue::dequeue() {
    if (queue.empty()) {
        return std::nullopt;
    }

    CommandData cmd = queue.front();
    queue.pop();
    inUse[static_cast<std::size_t>(cmd.type)] = false;
    return cmd;
}

bool CommandQueue::empty() const {
    return queue.empty();
}
