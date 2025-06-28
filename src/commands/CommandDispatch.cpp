#include "CommandDispatch.h"

#include "Command.h"
#include "CommandCenter.h"
#include "Logging.h"
#include "Telecommunication/BoardCommand.h"

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

void CommandDispatch::valve(Command& command) {
    if (!command.available()) {
        GCS_LOG_WARN("CommandDispatch: Couldn't send valve command, another one is already being processed.");
        return;
    }

    BoardCommand* boardCommand = (BoardCommand*) command.data;
    boardCommand->fields.crc = 1;
    boardCommand->fields.header.value = 1234;
    boardCommand->fields.value = 5678;

    command.ready(sizeof(BoardCommand));
}
