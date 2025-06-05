#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

#include "CircularBuffer.hpp"
#include "PacketFramer.h"

class PacketReceiver {
public:
    bool receiveByte(uint8_t c);
    bool getPacket(uint8_t* recv);
    bool dumpNextPacket();
    size_t nextPacketSize() const;

private:
    CircularBuffer<> buf;
    PacketFramer pf{buf};
};

#endif // PACKETRECEIVER_H
