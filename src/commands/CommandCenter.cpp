#include "CommandCenter.h"

#include "Command.h"

namespace CommandCenter {
Command command;
} // namespace CommandCenter

void CommandCenter::processCommands() {
    command.process();
}
