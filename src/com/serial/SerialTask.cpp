#include "SerialTask.h"

#include "CommandCenter.h"
#include "GSDataCenter.h"
#include "PacketProcessing.h"
#include "PacketRateMonitor.h"
#include "PacketReceiver.h"
#include "PlotData.h"
#include "SerialCom.h"
#include "SerialControl.h"
#include "SerialFailureMonitor.h"

#include <cmath>

namespace SerialTask {
PacketRateMonitor packetRateMonitor;
PacketReceiver packetReceiver;
SerialFailureMonitor serialFailureMonitor;
SerialCom com(packetRateMonitor, packetReceiver, serialFailureMonitor);
std::thread thread;
std::chrono::steady_clock::time_point timeLastUpdate = std::chrono::steady_clock::now();
std::atomic<bool> running = false;
std::atomic<bool> shouldStop = false;
} // namespace SerialTask

void addSineWavePoint() {
    static float t = 0.0f;               // time in seconds (or any unit)
    static const float dt = 0.01f;       // time step per call
    static const float frequency = 1.0f; // Hz
    static const float amplitude = 1.0f;

    float x = t;
    float y = amplitude * std::sin(2.0f * 3.14159265f * frequency * t);

    GSDataCenter::Thermistor1PlotData.addData(x, y);

    t += dt;
}

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

        addSineWavePoint();

        timeLastUpdate = now;
        // SerialControl::startComIfNeeded();
        // SerialControl::readIncomingBytesAtSetRate();
        // PacketProcessing::processIncomingPacket();
        // CommandCenter::processCommand();
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
    return (size_t) duration.count();
}
