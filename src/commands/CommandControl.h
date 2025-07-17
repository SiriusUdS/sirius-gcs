#ifndef COMMANDCONTROL_H
#define COMMANDCONTROL_H

#include "CommandTypes.h"

namespace CommandControl {
void sendCommand(CommandType type, uint32_t value);
void processCommands();
} // namespace CommandControl

#endif // COMMANDCONTROL_H
