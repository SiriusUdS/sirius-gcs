#ifndef COMMANDDISPATCH_H
#define COMMANDDISPATCH_H

class Command;

namespace CommandDispatch {
void test();
void valve(Command& valveCommand, size_t percentageOpen);
} // namespace CommandDispatch

#endif // COMMANDDISPATCH_H
