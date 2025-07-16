#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "CommandType.h"

#include <array>
#include <optional>
#include <queue>

struct CommandData {
    CommandType type;
    uint32_t value;
};

class CommandQueue {
public:
    bool enqueue(CommandType type, uint32_t value);
    std::optional<CommandData> dequeue();
    bool empty() const;

private:
    std::queue<CommandData> queue;
    std::array<bool, static_cast<std::size_t>(CommandType::Count)> inUse{};
};

#endif // COMMANDQUEUE_H
