#ifndef COMMANDTYPES_H
#define COMMANDTYPES_H

#include "Telecommunication/BoardCommand.h"
#include "Telecommunication/PacketHeaderVariable.h"

enum class CommandType : size_t {
    FillValve,
    DumpValve,
    NosHeatPad,
    IpaHeatPad,
    FillHeatPad,
    DumpHeatPad,
    Abort,
    Reset,

    Count // Always keep this as the last element
};

/**
 * @struct Command
 * @brief Represents a command with its type and value.
 */
struct Command {
    CommandType type; ///< Type of the command.
    uint32_t value;   ///< Value of the command.
};

enum class ValveCommandType { Fill = FILLING_STATION_COMMAND_CODE_OPEN_FILL_VALVE_PCT, Dump = FILLING_STATION_COMMAND_CODE_OPEN_DUMP_VALVE_PCT };

enum class HeatPadCommandType {
    Nos = ENGINE_COMMAND_CODE_SET_NOS_VALVE_HEATER_POWER_PCT,
    Ipa = ENGINE_COMMAND_CODE_SET_IPA_VALVE_HEATER_POWER_PCT,
    Fill = FILLING_STATION_COMMAND_CODE_SET_FILL_VALVE_HEATER_POWER_PCT,
    Dump = FILLING_STATION_COMMAND_CODE_SET_DUMP_VALVE_HEATER_POWER_PCT
};

enum class BoardType { Engine = ENGINE_BOARD_ID, FillingStation = FILLING_STATION_BOARD_ID };

#endif // COMMANDTYPES_H
