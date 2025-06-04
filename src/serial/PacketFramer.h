#ifndef PACKETFRAMER_H
#define PACKETFRAMER_H

#include "Constants.h"

#include <queue>

template <size_t>
class CircularBuffer;

class PacketFramer {
public:
    PacketFramer(const CircularBuffer<Constants::RECV_BUF_SIZE>& buf);
    void tryFrame();
    bool packetAvailable() const;
    size_t consumeNextPacketSize();
    void byteWritten();
    void clear();

private:
    bool checkForPacketStart();
    bool checkForTelemetryPacketStart();
    bool getHeaderFromBuf(size_t headerSize);

    const CircularBuffer<Constants::RECV_BUF_SIZE>& buf;
    uint8_t headerBuf[Constants::RECV_PACKET_MAX_HEADER_SIZE];
    size_t currentPacketSize{};
    std::queue<size_t> availablePacketSizesQueue{};
    bool readingValidPacket{};
};

#endif // PACKETFRAMER_H
