#include "SerialTask.h"

#include "CommandCenter.h"
#include "PacketProcessing.h"
#include "SerialControl.h"

namespace SerialTask {
SerialCom com;
std::thread thread;
std::chrono::steady_clock::time_point timeLastUpdate = std::chrono::steady_clock::now();
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
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeLastUpdate);
        if (duration.count() < (1000.f / Constants::SERIAL_TASK_LOOPS_PER_SECOND)) {
            continue;
        }

        timeLastUpdate = now;
        SerialControl::startComIfNeeded();
        SerialControl::readIncomingBytesAtSetRate();
        PacketProcessing::processIncomingPacket();
        CommandCenter::processCommands();
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
    auto duration = std::chrono::duration<double>(now - timeLastUpdate);
    return duration.count();
}
