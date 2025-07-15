#include "CommandCenter.h"

#include "Command.h"

namespace CommandCenter {
Command command;
Command valve1Command;
Command valve2Command;
Command valve3Command;
Command heatPadCommand;
} // namespace CommandCenter

void CommandCenter::processCommands() {
    command.process();
    valve1Command.process();
    valve2Command.process();
    valve3Command.process();
    heatPadCommand.process();
}
