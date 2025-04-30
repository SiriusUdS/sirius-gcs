#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include "Command.h"

namespace CommandCenter {
bool isValidId(size_t commandId);
size_t reserveSlot();
bool freeSlot(size_t commandId);
Command* get(size_t commandId);

extern Command commands[Constants::COMMAND_STORAGE_MAX_SIZE];
} // namespace CommandCenter

#endif // COMMANDCENTER_H
