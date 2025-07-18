#include "CommandControl.h"

#include "CRC.h"
#include "CommandQueue.h"
#include "Logging.h"
#include "SerialCom.h"
#include "SerialTask.h"
#include "Telecommunication/BoardCommand.h"
#include "Telecommunication/PacketHeaderVariable.h"
#include "Timer.h"

namespace CommandControl {
/**
 * @enum State
 * @brief Represents the state of the command control.
 */
enum class State {
    IDLE,    ///< No command is being sent
    SENDING, ///< A command is being sent
};

constexpr size_t MAX_DATA_SIZE = 256;                       ///< Maximum size of the command's data
constexpr size_t NUMBER_OF_TIMES_TO_SEND_SAME_COMMAND = 20; ///< Each command is sent this many times to improve communication with the boards
constexpr double TIME_BETWEEN_COMMAND_SENDS_SEC = 0.131;    ///< Wait this much between each command send

State state = State::IDLE;                 ///< Current state of the command
uint8_t data[MAX_DATA_SIZE] = {0};         ///< Current command data
size_t dataSize;                           ///< Size of the current command data
Timer lastTimeSentTimer;                   ///< Timer for the last time the command was sent
size_t timesSent{};                        ///< Number of times the same command has been sent
CommandQueue commandQueue;                 ///< Queue containing all future commands to be sent
std::optional<CommandData> currentCommand; ///< Current command being sent

void getNextCommand();
void setupValveCommand(ValveCommandType type);
void setupHeatPadCommand(HeatPadCommandType type);
void setupAbort();
void setupReset();
void finalizeCommandSetup(BoardCommand* cmd);
} // namespace CommandControl

void CommandControl::sendCommand(CommandType type, uint32_t value) {
    commandQueue.enqueue(type, value);
}

void CommandControl::processCommands() {
    switch (state) {
    case State::IDLE:
        getNextCommand();
        break;
    case State::SENDING:
        if (lastTimeSentTimer.getElapsedTimeInSeconds() < TIME_BETWEEN_COMMAND_SENDS_SEC) {
            break;
        }
        lastTimeSentTimer.reset();
        BoardCommand* formattedData = reinterpret_cast<BoardCommand*>(data);
        if (!SerialTask::com.write(data, dataSize)) {
            GCS_APP_LOG_ERROR("Command: Couldn't send command over serial communication.");
            break;
        }
        timesSent++;
        if (NUMBER_OF_TIMES_TO_SEND_SAME_COMMAND <= timesSent) {
            state = State::IDLE;
            currentCommand = std::nullopt;
            timesSent = 0;
        }
        break;
    }
}

void CommandControl::getNextCommand() {
    currentCommand = commandQueue.dequeue();

    if (!currentCommand.has_value()) {
        return;
    }

    switch (currentCommand.value().type) {
    case CommandType::FillValve:
        setupValveCommand(ValveCommandType::Fill);
        break;
    case CommandType::DumpValve:
        setupValveCommand(ValveCommandType::Dump);
        break;
    case CommandType::FillHeatPad:
        setupHeatPadCommand(HeatPadCommandType::Fill);
        break;
    case CommandType::DumpHeatPad:
        setupHeatPadCommand(HeatPadCommandType::Dump);
        break;
    case CommandType::Abort:
        setupAbort();
        break;
    case CommandType::Reset:
        setupReset();
        break;
    default:
        GCS_APP_LOG_ERROR("CommandControl: Invalid command type dequeued from command queue. Ignoring command.");
    }
}

void CommandControl::setupValveCommand(ValveCommandType type) {
    if (!currentCommand.has_value()) {
        GCS_APP_LOG_ERROR("CommandControl: Couldn't setup valve command, no command available.");
        return;
    }

    CommandData& command = currentCommand.value();
    uint32_t percentageOpen = command.value;

    if (percentageOpen > 100) {
        GCS_APP_LOG_WARN("CommandDispatch: Invalid valve percentage: {}. Must be between 0 and 100.", percentageOpen);
        return;
    }

    BoardCommand* boardCommand = reinterpret_cast<BoardCommand*>(data);
    boardCommand->fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    boardCommand->fields.header.bits.commandCode = static_cast<int>(type);
    boardCommand->fields.header.bits.commandIndex = 0;
    boardCommand->fields.header.bits.type = BOARD_COMMAND_UNICAST_TYPE_CODE;
    boardCommand->fields.value = percentageOpen;

    finalizeCommandSetup(boardCommand);
}

void CommandControl::setupHeatPadCommand(HeatPadCommandType type) {
    if (!currentCommand.has_value()) {
        GCS_APP_LOG_ERROR("CommandControl: Couldn't setup heat pad command, no command available.");
        return;
    }

    CommandData& command = currentCommand.value();
    uint32_t percentageOpen = command.value;

    if (percentageOpen > 100) {
        GCS_APP_LOG_WARN("CommandDispatch: Invalid heat pad percentage: {}. Must be between 0 and 100.", percentageOpen);
        return;
    }

    BoardCommand* boardCommand = reinterpret_cast<BoardCommand*>(data);
    boardCommand->fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    boardCommand->fields.header.bits.commandCode = static_cast<int>(type);
    boardCommand->fields.header.bits.commandIndex = 0;
    boardCommand->fields.header.bits.type = BOARD_COMMAND_UNICAST_TYPE_CODE;
    boardCommand->fields.value = percentageOpen;

    finalizeCommandSetup(boardCommand);
}

void CommandControl::setupAbort() {
    if (!currentCommand.has_value()) {
        GCS_APP_LOG_ERROR("CommandControl: Couldn't setup abort command, no command available.");
        return;
    }

    CommandData& command = currentCommand.value();

    BoardCommand* boardCommand = reinterpret_cast<BoardCommand*>(data);
    boardCommand->fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    boardCommand->fields.header.bits.commandCode = BOARD_COMMAND_CODE_ABORT;
    boardCommand->fields.header.bits.commandIndex = 0;
    boardCommand->fields.header.bits.type = BOARD_COMMAND_BROADCAST_TYPE_CODE;
    boardCommand->fields.value = 0;

    finalizeCommandSetup(boardCommand);
}

void CommandControl::setupReset() {
    if (!currentCommand.has_value()) {
        GCS_APP_LOG_ERROR("CommandControl: Couldn't setup reset command, no command available.");
        return;
    }

    CommandData& command = currentCommand.value();

    BoardCommand* boardCommand = reinterpret_cast<BoardCommand*>(data);
    boardCommand->fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    boardCommand->fields.header.bits.commandCode = BOARD_COMMAND_CODE_RESET;
    boardCommand->fields.header.bits.commandIndex = 0;
    boardCommand->fields.header.bits.type = BOARD_COMMAND_BROADCAST_TYPE_CODE;
    boardCommand->fields.value = 0;

    finalizeCommandSetup(boardCommand);
}

void CommandControl::finalizeCommandSetup(BoardCommand* cmd) {
    cmd->fields.crc = CRC::computeCrc(cmd->data, sizeof(BoardCommand) - sizeof(cmd->fields.crc));
    dataSize = sizeof(BoardCommand);
    state = State::SENDING;
}
