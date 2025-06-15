#ifndef SERIALTASK_H
#define SERIALTASK_H

#include <atomic>
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
} // namespace SerialTask

#endif // SERIALTASK_H
