#include "CommandDispatch.h"

#include "Command.h"
#include "CommandCenter.h"
#include "Logging.h"
#include "Telecommunication/BoardCommand.h"

void CommandDispatch::test() {
    Command& command = CommandCenter::command;

    if (!command.available()) {
        GCS_LOG_WARN("CommandDispatch: Couldn't send command, another command is already being processed.");
        return;
    }

    BoardCommand* boardCommand = (BoardCommand*) command.data;
    boardCommand->fields.crc = 1;
    boardCommand->fields.header.value = 1234;
    boardCommand->fields.value = 5678;

    command.ready(sizeof(BoardCommand));
}
