#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "Constants.h"
#include "PacketRateMonitor.h"
#include "PacketReceiver.h"
#include "SerialFailureMonitor.h"

#include <ceserial.h>
#include <chrono>
#include <mutex>

/**
 * @class SerialCom
 * @brief Handles serial communication to receive packets
 */
class SerialCom {
public:
    void start();
    bool read();
    bool write(uint8_t* msg, size_t size);
    bool comOpened();
    bool comWorking();
    size_t getPacket(uint8_t* recv);
    size_t nextPacketSize();
    bool dumpNextPacket();
    size_t packetsReadPerSecond();
    void shutdown();

private:
    ceSerial com;
    SerialFailureMonitor sfm;
    PacketRateMonitor prm;
    PacketReceiver pr;
};

#endif // SERIALCOM_H
