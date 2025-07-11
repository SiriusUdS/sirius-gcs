#include "SerialTask.h"

#include "ComDiscoveryWindows.h"
#include "ComPortSelector.h"
#include "CommandCenter.h"
#include "IntervalTimer.h"
#include "PacketProcessing.h"
#include "PacketRateMonitor.h"
#include "PacketReceiver.h"
#include "SerialCom.h"
#include "SerialConfig.h"
#include "SerialControl.h"
#include "SerialStateMonitor.h"

namespace SerialTask {
PacketRateMonitor packetRateMonitor;
PacketRateMonitor engineTelemetryPacketRateMonitor;
PacketRateMonitor fillingStationTelemetryPacketRateMonitor;
PacketRateMonitor gsControlPacketRateMonitor;
PacketRateMonitor engineStatusPacketRateMonitor;
PacketRateMonitor fillingStationStatusPacketRateMonitor;
PacketReceiver packetReceiver;
SerialStateMonitor serialStateMonitor;
ComDiscoveryWindows comDiscovery;
ComPortSelector comPortSelector(comDiscovery);
SerialCom com(comPortSelector, packetRateMonitor, packetReceiver, serialStateMonitor);

IntervalTimer intervalTimer(std::chrono::milliseconds(1000 / SerialConfig::SERIAL_TASK_LOOPS_PER_SECOND));
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
        intervalTimer.waitUntilNextInterval();
        SerialControl::startComIfNeeded();
        SerialControl::readIncomingBytesAtSetRate();
        PacketProcessing::processIncomingPackets();
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
