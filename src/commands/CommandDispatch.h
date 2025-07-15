#ifndef COMMANDDISPATCH_H
#define COMMANDDISPATCH_H

#include "Telecommunication/BoardCommand.h"

class Command;

namespace CommandDispatch {
enum class HeatPadCommandType {
    NOS = ENGINE_COMMAND_CODE_SET_NOS_VALVE_HEATER_POWER_PCT,
    IPA = ENGINE_COMMAND_CODE_SET_IPA_VALVE_HEATER_POWER_PCT,
    FILL = FILLING_STATION_COMMAND_CODE_SET_FILL_VALVE_HEATER_POWER_PCT,
    DUMP = FILLING_STATION_COMMAND_CODE_SET_DUMP_VALVE_HEATER_POWER_PCT
};

void test();
void valve(Command& valveCommand, size_t percentageOpen);
void heatPad(HeatPadCommandType heatPadCommandType, size_t percentageOpen);
} // namespace CommandDispatch

#endif // COMMANDDISPATCH_H
