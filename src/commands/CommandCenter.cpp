#include "CommandCenter.h"

#include "Command.h"

namespace CommandCenter {
Command valveCommand;
Command heatPadCommand;
} // namespace CommandCenter

void CommandCenter::processCommands() {
    valveCommand.process();
    heatPadCommand.process();
}
