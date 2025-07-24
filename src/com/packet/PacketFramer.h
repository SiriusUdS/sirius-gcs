#ifndef PACKETFRAMER_H
#define PACKETFRAMER_H

#include "PacketCircularBuffer.h"

#include <queue>

/**
 * @class PacketFramer
 * @brief Frames packets by reading the bytes written into a circular buffer
 */
class PacketFramer {
public:
    PacketFramer(const PacketCircularBuffer& buf);
    void tryFrame();
    size_t consumeNextPacketSize();
    void byteWritten();
    void clear();
    bool packetAvailable() const;
    size_t peekNextPacketSize() const;

private:
    bool checkForPacketStart();
    bool checkForTelemetryPacketStart();
    bool getHeaderFromBuf(size_t headerSize);

    static constexpr size_t MAX_HEADER_SIZE = 4;

    const PacketCircularBuffer& buf;
    uint8_t headerBuf[MAX_HEADER_SIZE];
    size_t currentPacketSize{};
    std::queue<size_t> availablePacketSizesQueue{};
    bool readingValidPacket{};
};

#endif // PACKETFRAMER_H
