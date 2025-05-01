#ifndef COMMAND_H
#define COMMAND_H

#include "Constants.h"

#include <stdint.h>

enum CommandState { NONE, READY, SENT };

struct Command {
    CommandState state;
    uint8_t data[Constants::COMMAND_MAX_SIZE];
};

#endif // COMMAND_H
