#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include "Command.h"

#include <mutex>

namespace CommandCenter {
Command* get(size_t commandSlotId);
size_t reserveSlot();
bool ready(size_t commandSlotId);
bool freeSlot(size_t commandSlotId);
bool processAck(size_t commandSlotId);
void processCommands();
bool isValidSlotId(size_t commandSlotId);

extern Command commands[Constants::COMMAND_STORAGE_MAX_SIZE];
extern std::mutex mtx;
} // namespace CommandCenter

#endif // COMMANDCENTER_H
