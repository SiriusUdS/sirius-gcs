#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "CommandTypes.h"

#include <array>
#include <optional>
#include <queue>

class CommandQueue {
public:
    void enqueue(CommandType type, uint32_t value);
    std::optional<Command> dequeue();
    bool empty() const;

private:
    std::queue<CommandType> pendingTypes;
    std::array<std::optional<uint32_t>, static_cast<std::size_t>(CommandType::Count)> values{};
};

#endif // COMMANDQUEUE_H
