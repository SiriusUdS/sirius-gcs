#include "CommandCenter.h"

#include "Command.h"

namespace CommandCenter {
Command command;
Command fillValveCommand;
Command dumpValveCommand;
Command heatPadCommand;
} // namespace CommandCenter

void CommandCenter::processCommands() {
    command.process();
    fillValveCommand.process();
    dumpValveCommand.process();
    heatPadCommand.process();
}
