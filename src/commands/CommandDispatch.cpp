#include "CommandDispatch.h"

#include "Command.h"
#include "CommandCenter.h"
#include "Logging.h"
#include "Telecommunication/BoardCommand.h"

void CommandDispatch::test() {
    if (!CommandCenter::available()) {
        GCS_LOG_WARN("CommandDispatch: Couldn't send command, another command is already being processed.");
        return;
    }

    BoardCommand* boardCommand = (BoardCommand*) CommandCenter::get().data;
    boardCommand->fields.crc = 1;
    boardCommand->fields.header.value = 1234;
    boardCommand->fields.value = 5678;

    CommandCenter::ready(sizeof(BoardCommand));
}
