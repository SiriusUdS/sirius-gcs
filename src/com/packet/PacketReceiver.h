#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

#include "PacketCircularBuffer.h"
#include "PacketFramer.h"

/**
 * @class PacketReceiver
 * @brief Handles packet reception
 */
class PacketReceiver {
public:
    bool receiveByte(uint8_t byte);
    bool getPacket(uint8_t* recv);
    uint8_t* getBuffer();
    bool dumpNextPacket();
    size_t nextPacketSize() const;

private:
    PacketCircularBuffer buf;
    PacketFramer pf{buf};
};

#endif // PACKETRECEIVER_H
