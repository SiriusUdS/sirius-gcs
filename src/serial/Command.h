#ifndef COMMAND_H
#define COMMAND_H

#include "Constants.h"

#include <stdint.h>

struct Command {
    bool valid{};
    bool sent{};
    uint8_t data[Constants::COMMAND_MAX_SIZE];
};

#endif // COMMAND_H
