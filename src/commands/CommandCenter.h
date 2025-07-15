#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

class Command;

namespace CommandCenter {
extern Command command;
extern Command fillValveCommand;
extern Command dumpValveCommand;
extern Command heatPadCommand;

void processCommands();
} // namespace CommandCenter

#endif // COMMANDCENTER_H
