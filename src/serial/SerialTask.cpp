#include "SerialTask.h"

#include "CommandCenter.h"
#include "PacketProcessing.h"
#include "SerialControl.h"

namespace SerialTask {
SerialCom com;
std::thread thread;
std::chrono::steady_clock::time_point lastCompletedTaskLoopIteration = std::chrono::steady_clock::now();
std::atomic<bool> running = false;
std::atomic<bool> shouldStop = false;
} // namespace SerialTask

void SerialTask::start() {
    if (running) {
        return;
    }

    shouldStop = false;
    thread = std::thread(&SerialTask::execute);
    running = true;
}

void SerialTask::execute() {
    while (!shouldStop) {
        SerialControl::startComIfNeeded();
        SerialControl::readIncomingBytesAtSetRate();
        PacketProcessing::processIncomingPacket();
        CommandCenter::processCommands();
        lastCompletedTaskLoopIteration = std::chrono::steady_clock::now();
    }
}

void SerialTask::restart() {
    stop();
    start();
}

void SerialTask::stop() {
    if (!running) {
        return;
    }

    shouldStop = true;
    if (thread.joinable()) {
        thread.join();
    }
    running = false;
}

size_t SerialTask::secondsSinceLastUpdate() {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastCompletedTaskLoopIteration);
    return duration.count();
}
