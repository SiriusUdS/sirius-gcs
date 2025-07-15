#ifndef SERIALTASK_H
#define SERIALTASK_H

#include <atomic>
#include <thread>

class BoardComStateMonitor;
class ComDiscoveryWindows;
class ComPortSelector;
class IntervalTimer;
class PacketRateMonitor;
class PacketReceiver;
class SerialCom;

namespace SerialTask {
void start();
void execute();
void restart();
void stop();

extern PacketRateMonitor packetRateMonitor;
extern PacketRateMonitor engineTelemetryPacketRateMonitor;
extern PacketRateMonitor fillingStationTelemetryPacketRateMonitor;
extern PacketRateMonitor gsControlPacketRateMonitor;
extern PacketRateMonitor engineStatusPacketRateMonitor;
extern PacketRateMonitor fillingStationStatusPacketRateMonitor;
extern PacketReceiver packetReceiver;
extern BoardComStateMonitor motorBoardComStateMonitor;
extern BoardComStateMonitor fillingStationBoardComStateMonitor;
extern BoardComStateMonitor gsControlBoardComStateMonitor;
extern ComDiscoveryWindows comDiscovery;
extern ComPortSelector comPortSelector;
extern SerialCom com;
} // namespace SerialTask

#endif // SERIALTASK_H
