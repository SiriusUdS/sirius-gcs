#include "PacketFramer.h"

#include "Logging.h"
#include "PacketCircularBuffer.h"
#include "Telecommunication/TelemetryHeader.h"

/**
 * @brief Constructs a packet framer by specifing the circular buffer to read the bytes from
 */
PacketFramer::PacketFramer(const PacketCircularBuffer& buf) : buf{buf}, headerBuf{{0}} {
}

/**
 * @brief Try to frame a packet, starting from the last byte received in the circular buffer
 */
std::optional<size_t> PacketFramer::tryFrame() {
    if (!checkForPacketStart()) {
        return;
    }

    if (readingValidPacket) {
        size_t tempPacketSize = currentPacketSize;
        currentPacketSize = 0;
        return tempPacketSize;

    } else {
        readingValidPacket = true;
        currentPacketSize = 0;
        return std::nullopt;
    }
}

/**
 * @brief Notify the packet framer that a byte was written in the circular buffer to keep track of the current packet's size
 */
void PacketFramer::byteWritten() {
    currentPacketSize++;
}

/**
 * @brief Clears all packets detected from the packet framer
 */
void PacketFramer::clear() {
    currentPacketSize = 0;
    readingValidPacket = false;
}

/**
 * @brief Check if a packet was received in the circular buffer, starting from the last byte written in the buffer
 * @returns True if a packet was detected, else false
 */
bool PacketFramer::checkForPacketStart() {
    if (currentPacketSize < sizeof(TelemetryHeader) || !getHeaderFromBuf(sizeof(TelemetryHeader))) {
        return false;
    }

    TelemetryHeader* header = (TelemetryHeader*) headerBuf;
    return header->bits.type == TELEMETRY_TYPE_CODE || header->bits.type == STATUS_TYPE_CODE;
}

/**
 * @brief Copies the header of packet written in the circular buffer to an internal buffer in the packet framer for further processing
 * @returns True if the header was successfully copied in the internal buffer, else false
 */
bool PacketFramer::getHeaderFromBuf(size_t headerSize) {
    if (headerSize > MAX_HEADER_SIZE) {
        GCS_APP_LOG_ERROR(
          "PacketFramer: Tried to get header from circular buffer, but the header of size {} is bigger than the maximum header size accepted of {}.",
          headerSize,
          MAX_HEADER_SIZE);
        return false;
    }

    if (headerSize > currentPacketSize) {
        GCS_APP_LOG_ERROR(
          "PacketFramer: Tried to get header from circular buffer, but the header of size {} is bigger than the packet being currently "
          "read of size {}.",
          headerSize,
          currentPacketSize);
        return false;
    }

    for (size_t i = 0; i < headerSize; i++) {
        std::optional<uint8_t> byte = buf.peekBack(headerSize - i - 1);
        if (!byte.has_value()) {
            GCS_APP_LOG_ERROR("PacketFramer: Tried to get header from circular buffer, but not enough bytes are available.");
            return false;
        }
        headerBuf[i] = byte.value();
    }

    return true;
}
