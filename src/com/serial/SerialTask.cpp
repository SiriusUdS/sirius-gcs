#include "SerialTask.h"

#include "CommandCenter.h"
#include "IntervalTimer.h"
#include "PacketProcessing.h"
#include "PacketRateMonitor.h"
#include "PacketReceiver.h"
#include "SerialCom.h"
#include "SerialControl.h"
#include "SerialStateMonitor.h"

namespace SerialTask {
static constexpr size_t SERIAL_TASK_LOOPS_PER_SECOND = 20;

PacketRateMonitor packetRateMonitor;
PacketReceiver packetReceiver;
SerialStateMonitor serialFailureMonitor;
SerialCom com(packetRateMonitor, packetReceiver, serialFailureMonitor);
IntervalTimer intervalTimer(std::chrono::milliseconds(1000 / SERIAL_TASK_LOOPS_PER_SECOND));
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
        if (intervalTimer.getElapsedCount() == 0) {
            continue;
        }

        intervalTimer.resetElapsedCount();
        SerialControl::startComIfNeeded();
        SerialControl::readIncomingBytesAtSetRate();
        PacketProcessing::processIncomingPacket();
        CommandCenter::processCommand();
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
