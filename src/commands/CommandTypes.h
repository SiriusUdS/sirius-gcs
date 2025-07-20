#ifndef COMMANDTYPES_H
#define COMMANDTYPES_H

#include "Telecommunication/BoardCommand.h"

enum class CommandType : size_t {
    FillValve,
    DumpValve,
    FillHeatPad,
    DumpHeatPad,
    Abort,
    Reset,

    Count // Always keep this as the last element
};

struct Command {
    CommandType type;
    uint32_t value;
};

enum class ValveCommandType { Fill = FILLING_STATION_COMMAND_CODE_OPEN_FILL_VALVE_PCT, Dump = FILLING_STATION_COMMAND_CODE_OPEN_DUMP_VALVE_PCT };

enum class HeatPadCommandType {
    Fill = FILLING_STATION_COMMAND_CODE_SET_FILL_VALVE_HEATER_POWER_PCT,
    Dump = FILLING_STATION_COMMAND_CODE_SET_DUMP_VALVE_HEATER_POWER_PCT
};

#endif // COMMANDTYPES_H
