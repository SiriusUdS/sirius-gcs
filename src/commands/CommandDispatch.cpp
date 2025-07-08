#include "CommandDispatch.h"

#include "Command.h"
#include "CommandCenter.h"
#include "Logging.h"
#include "Telecommunication/BoardCommand.h"
#include "Telecommunication/PacketHeaderVariable.h"

void CommandDispatch::test() {
    Command& command = CommandCenter::command;

    if (!command.available()) {
        GCS_LOG_WARN("CommandDispatch: Couldn't send test command, another one is already being processed.");
        return;
    }

    BoardCommand* boardCommand = (BoardCommand*) command.data;
    boardCommand->fields.crc = 1;
    boardCommand->fields.header.value = 1234;
    boardCommand->fields.value = 5678;

    command.ready(sizeof(BoardCommand));
}

void CommandDispatch::valve(Command& command, size_t percentageOpen) {
    if (!command.available()) {
        GCS_LOG_DEBUG("CommandDispatch: Couldn't send valve command, another one is already being processed.");
        return;
    } else {
        GCS_LOG_DEBUG("AVAILABLE WITH VALUE {}", percentageOpen);
    }

    if (percentageOpen > 100) {
        GCS_LOG_WARN("CommandDispatch: Invalid valve percentage: {}. Must be between 0 and 100.", percentageOpen);
        return;
    }

    BoardCommand* boardCommand = (BoardCommand*) command.data;
    boardCommand->fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    boardCommand->fields.header.bits.commandCode = ENGINE_COMMAND_CODE_OPEN_VALVE;
    boardCommand->fields.header.bits.commandIndex = 0;
    boardCommand->fields.header.bits.type = BOARD_COMMAND_TYPE_CODE;

    boardCommand->fields.crc = 0;
    boardCommand->fields.value = (uint32_t) percentageOpen;

    command.ready(sizeof(BoardCommand));
}
