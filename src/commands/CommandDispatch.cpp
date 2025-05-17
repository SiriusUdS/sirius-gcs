#include "CommandDispatch.h"

#include "CommandCenter.h"
#include "Logging.h"
#include "Telecommunication/BoardCommand.h"

void CommandDispatch::test() {
    size_t commandSlotId = CommandCenter::reserveSlot();
    if (!CommandCenter::isValidSlotId(commandSlotId)) {
        GCS_LOG_WARN("CommandDispatch: Couldn't send command, no command slot available.");
        return;
    }

    Command* command = (Command*) CommandCenter::get(commandSlotId);
    if (!command) {
        GCS_LOG_WARN("CommandDispatch: Couldn't send command, command slot pointer is null.");
        return;
    }
    command->size = sizeof(BoardCommand);

    BoardCommand* boardCommand = (BoardCommand*) command->data;
    boardCommand->fields.crc = 1;
    boardCommand->fields.header.value = 1234;
    boardCommand->fields.value = 5678;

    CommandCenter::ready(commandSlotId);
}
