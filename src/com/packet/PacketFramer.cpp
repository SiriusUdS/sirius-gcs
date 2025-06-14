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
void PacketFramer::tryFrame() {
    if (!checkForPacketStart()) {
        return;
    }

    if (readingValidPacket) {
        availablePacketSizesQueue.push(currentPacketSize);
    } else {
        readingValidPacket = true;
    }
    currentPacketSize = 0;
}

/**
 * @brief Retrieve the size of the next packet and remove it from the packet framer
 * @returns The size of the next packet
 */
size_t PacketFramer::consumeNextPacketSize() {
    if (!availablePacketSizesQueue.size()) {
        GCS_LOG_WARN("PacketFramer: Tried to consume next packet size, but no packets available.");
        return 0;
    }

    size_t size = availablePacketSizesQueue.front();
    availablePacketSizesQueue.pop();
    return size;
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
    while (!availablePacketSizesQueue.empty()) {
        availablePacketSizesQueue.pop();
    }
}

/**
 * @brief Check if a packet was received in the circular buffer, starting from the last byte written in the buffer
 * @returns True if a packet was detected, else false
 */
bool PacketFramer::checkForPacketStart() {
    return checkForTelemetryPacketStart();

    // TODO: Check for GS Control packet
}

/**
 * @brief Check if a telemetry packet was received in the circular buffer, starting from the last byte written in the buffer
 * @returns True if a packet was detected, else false
 */
bool PacketFramer::checkForTelemetryPacketStart() {
    static const uint32_t TELEMETRY_HEADER_CODES[] = {TELEMETRY_TYPE_CODE, STATUS_TYPE_CODE};

    if (currentPacketSize < sizeof(TelemetryHeader)) {
        return {};
    }

    for (const uint32_t headerCode : TELEMETRY_HEADER_CODES) {
        getHeaderFromBuf(sizeof(TelemetryHeader));
        TelemetryHeader* header = (TelemetryHeader*) headerBuf;
        if (headerCode == header->bits.type) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Copies the header of packet written in the circular buffer to an internal buffer in the packet framer for further processing
 * @returns True if the header was successfully copied in the internal buffer, else false
 */
bool PacketFramer::getHeaderFromBuf(size_t headerSize) {
    if (headerSize > MAX_HEADER_SIZE) {
        GCS_LOG_WARN(
          "PacketFramer: Tried to get header from circular buffer, but the header of size {} is bigger than the maximum header size accepted of {}.",
          headerSize, MAX_HEADER_SIZE);
        return false;
    }

    if (headerSize > currentPacketSize) {
        GCS_LOG_WARN("PacketFramer: Tried to get header from circular buffer, but the header of size {} is bigger than the packet being currently "
                     "read of size {}.",
                     headerSize, currentPacketSize);
        return false;
    }

    const size_t READ_AVAILABLE = buf.readAvailable();
    if (headerSize > READ_AVAILABLE) {
        GCS_LOG_WARN(
          "PacketFramer: Tried to get header from buffer, but the header of size {} is bigger than the packet being currently read of size {}.",
          headerSize, READ_AVAILABLE);
        return false;
    }

    for (size_t i = 0; i < headerSize; i++) {
        std::optional<uint8_t> byte = buf.peekBack(headerSize - i - 1);
        if (!byte.has_value()) {
            GCS_LOG_WARN("PacketFramer: Tried to get header from circular buffer, but not enough bytes are available.");
            return false;
        }
        headerBuf[i] = byte.value();
    }

    return true;
}

/**
 * @brief Returns whether a packet is available
 * @returns True if a packet is available, else false
 */
bool PacketFramer::packetAvailable() const {
    return availablePacketSizesQueue.size();
}

/**
 * @brief Get the size of the next packet without removing it from the packet framer
 * @returns Size of the next packet
 */
size_t PacketFramer::peekNextPacketSize() const {
    if (!availablePacketSizesQueue.size()) {
        return 0;
    }
    return availablePacketSizesQueue.front();
}
