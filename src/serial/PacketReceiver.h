#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

#include "CircularBuffer.hpp"
#include "PacketFramer.h"

/**
 * @class PacketReceiver
 * @brief Handles packet reception
 */
class PacketReceiver {
public:
    bool receiveByte(uint8_t byte);
    bool getPacket(uint8_t* recv);
    bool dumpNextPacket();
    size_t nextPacketSize() const;

private:
    CircularBuffer<> buf;
    PacketFramer pf{buf};
};

#endif // PACKETRECEIVER_H
