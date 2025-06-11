#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <ceserial.h>

class PacketRateMonitor;
class PacketReceiver;
class SerialFailureMonitor;

/**
 * @class SerialCom
 * @brief Handles serial communication to receive packets
 */
class SerialCom {
public:
    SerialCom(PacketRateMonitor& prm, PacketReceiver& pr, SerialFailureMonitor& sfm);
    void start();
    bool read();
    bool write(uint8_t* msg, size_t size);
    bool comOpened();
    size_t getPacket(uint8_t* recv);
    void shutdown();

private:
    ceSerial com;
    PacketRateMonitor& packetRateMonitor;
    PacketReceiver& packetReceiver;
    SerialFailureMonitor& serialFailureMonitor;
};

#endif // SERIALCOM_H
