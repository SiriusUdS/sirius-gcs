#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include "Command.h"

namespace CommandCenter {
Command* get(size_t commandSlotId);
size_t reserveSlot();
bool freeSlot(size_t commandSlotId);
bool processAck(size_t commandSlotId);
void processCommands();
bool isValidSlotId(size_t commandSlotId);

extern Command commands[Constants::COMMAND_STORAGE_MAX_SIZE];
} // namespace CommandCenter

#endif // COMMANDCENTER_H
