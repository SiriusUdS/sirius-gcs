#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

class Command;

namespace CommandCenter {
extern Command valveCommand;
extern Command heatPadCommand;

void processCommands();
} // namespace CommandCenter

#endif // COMMANDCENTER_H
