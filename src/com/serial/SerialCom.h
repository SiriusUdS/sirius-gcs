#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <ceserial.h>

class PacketRateMonitor;
class PacketReceiver;
class SerialStateMonitor;

/**
 * @class SerialCom
 * @brief Handles serial communication to receive packets
 */
class SerialCom {
public:
    SerialCom(PacketRateMonitor& prm, PacketReceiver& pr, SerialStateMonitor& sfm);
    void start();
    bool read();
    bool write(uint8_t* msg, size_t size);
    bool comOpened();
    bool getPacket(uint8_t* recv);
    uint8_t* getBuffer();
    void shutdown();

private:
    ceSerial com;
    PacketRateMonitor& packetRateMonitor;
    PacketReceiver& packetReceiver;
    SerialStateMonitor& serialFailureMonitor;
};

#endif // SERIALCOM_H
