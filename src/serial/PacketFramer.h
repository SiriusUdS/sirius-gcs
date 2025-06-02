#ifndef PACKETFRAMER_H
#define PACKETFRAMER_H

#include "Constants.h"

#include <optional>

class CircularBuffer;

class PacketFramer {
public:
    PacketFramer(CircularBuffer& buf);
    std::optional<uint32_t> checkForPacketStart();

private:
    bool getHeaderFromBuf(size_t headerSize);

    CircularBuffer& buf;
    uint8_t headerBuf[Constants::RECV_PACKET_MAX_HEADER_SIZE];
};

#endif // PACKETFRAMER_H
