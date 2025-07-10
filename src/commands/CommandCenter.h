#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

class Command;

namespace CommandCenter {
extern Command command;
extern Command valve1Command;
extern Command valve2Command;
extern Command valve3Command;
extern Command heatPadCommand;

void processCommands();
} // namespace CommandCenter

#endif // COMMANDCENTER_H
