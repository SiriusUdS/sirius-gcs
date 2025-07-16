#ifndef COMMANDDISPATCH_H
#define COMMANDDISPATCH_H

#include "Telecommunication/BoardCommand.h"

namespace CommandDispatch {
enum class HeatPadCommandType {
    FILL = FILLING_STATION_COMMAND_CODE_SET_FILL_VALVE_HEATER_POWER_PCT,
    DUMP = FILLING_STATION_COMMAND_CODE_SET_DUMP_VALVE_HEATER_POWER_PCT
};

void valve(int valveId, size_t percentageOpen);
void heatPad(HeatPadCommandType heatPadCommandType, size_t percentageOpen);
} // namespace CommandDispatch

#endif // COMMANDDISPATCH_H
