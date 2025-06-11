#ifndef SERIALTASK_H
#define SERIALTASK_H

#include <atomic>
#include <chrono>
#include <thread>

class SerialFailureMonitor;
class PacketRateMonitor;
class PacketReceiver;
class SerialCom;

namespace SerialTask {
void start();
void execute();
void restart();
void stop();
size_t secondsSinceLastUpdate();

extern PacketRateMonitor packetRateMonitor;
extern PacketReceiver packetReceiver;
extern SerialFailureMonitor serialFailureMonitor;
extern SerialCom com;
extern std::thread thread;
extern std::chrono::steady_clock::time_point timeLastUpdate;
extern std::atomic<bool> running;
extern std::atomic<bool> shouldStop;
} // namespace SerialTask

#endif // SERIALTASK_H
