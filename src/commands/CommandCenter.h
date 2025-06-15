#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

struct Command;

namespace CommandCenter {
Command& get();
bool available();
bool ready(size_t commandSize);
bool processAck();
void processCommand();
} // namespace CommandCenter

#endif // COMMANDCENTER_H
