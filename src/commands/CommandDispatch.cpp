#include "CommandDispatch.h"

#include "CRC.h"
#include "Command.h"
#include "CommandCenter.h"
#include "Logging.h"
#include "Telecommunication/PacketHeaderVariable.h"

void CommandDispatch::valve(int valveId, size_t percentageOpen) {
    Command& command = CommandCenter::valveCommand;

    if (!command.available()) {
        GCS_LOG_DEBUG("CommandDispatch: Couldn't send valve command, another one is already being processed.");
        return;
    } else {
        GCS_LOG_DEBUG("CommandDispatch: Sending valve command with percentage: {}.", percentageOpen);
    }

    if (percentageOpen > 100) {
        GCS_LOG_WARN("CommandDispatch: Invalid valve percentage: {}. Must be between 0 and 100.", percentageOpen);
        return;
    }

    BoardCommand* boardCommand = (BoardCommand*) command.data;
    boardCommand->fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    boardCommand->fields.header.bits.commandCode = valveId;
    boardCommand->fields.header.bits.commandIndex = 0;
    boardCommand->fields.header.bits.type = BOARD_COMMAND_UNICAST_TYPE_CODE;
    boardCommand->fields.value = (uint32_t) percentageOpen;
    boardCommand->fields.crc = CRC::computeCrc(boardCommand->data, sizeof(BoardCommand) - sizeof(boardCommand->fields.crc));

    const char* valveName = "unknown";
    switch (valveId) {
    case FILLING_STATION_COMMAND_CODE_OPEN_FILL_VALVE_PCT:
        valveName = "fill";
        break;
    case FILLING_STATION_COMMAND_CODE_OPEN_DUMP_VALVE_PCT:
        valveName = "dump";
        break;
    }

    if (!command.ready(sizeof(BoardCommand))) {
        GCS_LOG_WARN("CommandDispatch: Failed to prepare %s valve command for sending.", valveName);
    }
}

void CommandDispatch::heatPad(HeatPadCommandType heatPadCommandType, size_t percentageOpen) {
    Command& command = CommandCenter::heatPadCommand;

    if (!command.available()) {
        GCS_LOG_WARN("CommandDispatch: Couldn't send heat pad command, another one is already being processed.");
        return;
    }

    BoardCommand* boardCommand = (BoardCommand*) command.data;
    boardCommand->fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    boardCommand->fields.header.bits.commandCode = (uint32_t) heatPadCommandType;
    boardCommand->fields.header.bits.commandIndex = 0;
    boardCommand->fields.header.bits.type = BOARD_COMMAND_UNICAST_TYPE_CODE;
    boardCommand->fields.value = (uint32_t) percentageOpen;
    boardCommand->fields.crc = CRC::computeCrc(boardCommand->data, sizeof(BoardCommand) - sizeof(boardCommand->fields.crc));

    if (!command.ready(sizeof(BoardCommand))) {
        GCS_LOG_WARN("CommandDispatch: Failed to prepare heat pad command for sending.");
    }
}
