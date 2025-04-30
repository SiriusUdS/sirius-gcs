#ifndef COMMANDTRACKING_H
#define COMMANDTRACKING_H

#include "Constants.h"

#include <chrono>
#include <list>

namespace CommandTracking {
void addCommand();
void processAck();
void verifyAck();

struct SentCommand {
    std::chrono::time_point<std::chrono::steady_clock> timeSent;
    uint8_t data[Constants::COMMAND_MAX_SIZE];
};

extern std::list<SentCommand> sentCommands;
} // namespace CommandTracking

#endif // COMMANDTRACKING_H
