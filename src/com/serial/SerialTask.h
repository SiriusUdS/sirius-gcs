#ifndef SERIALTASK_H
#define SERIALTASK_H

#include <atomic>
#include <chrono>
#include <thread>

class SerialFailureMonitor;
class PacketRateMonitor;
class PacketReceiver;
class SerialCom;
class IntervalTimer;

namespace SerialTask {
void start();
void execute();
void restart();
void stop();

extern PacketRateMonitor packetRateMonitor;
extern PacketReceiver packetReceiver;
extern SerialFailureMonitor serialFailureMonitor;
extern SerialCom com;
extern IntervalTimer intervalTimer;
extern std::thread thread;
extern std::atomic<bool> running;
extern std::atomic<bool> shouldStop;
} // namespace SerialTask

#endif // SERIALTASK_H
