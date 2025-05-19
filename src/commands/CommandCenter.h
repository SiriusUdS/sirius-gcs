#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include "Command.h"

#include <mutex>

namespace CommandCenter {
Command& get();
bool available();
bool ready(size_t commandSize);
bool processAck();
void processCommand();

extern Command command;
extern std::mutex mtx;
} // namespace CommandCenter

#endif // COMMANDCENTER_H
