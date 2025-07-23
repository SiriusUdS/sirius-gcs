#include "PacketFramer.h"

#include "Logging.h"
#include "PacketCircularBuffer.h"
#include "SerialConfig.h"
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
    std::optional<PacketMetadata> optionalMetadata = tryGetNextPacketMetadata();

    if (!optionalMetadata.has_value()) {
        return;
    }

    if (readingValidPacket) {
        availablePacketMetadata.push(optionalMetadata.value());
    } else {
        readingValidPacket = true;
    }
    currentPacketSize = 0;
}

/**
 * @brief Retrieve the metadata of the next packet and remove it from the packet framer
 * @returns The metadata of the next packet
 */
std::optional<PacketMetadata> PacketFramer::consumeNextPacketMetadata() {
    if (!availablePacketMetadata.size()) {
        GCS_APP_LOG_WARN("PacketFramer: Tried to consume next packet size, but no packets available.");
        return std::nullopt;
    }

    PacketMetadata metadata = availablePacketMetadata.front();
    availablePacketMetadata.pop();
    return metadata;
}

/**
 * @brief Notify the packet framer that a byte was written in the circular buffer to keep track of the current packet's size
 */
void PacketFramer::byteWritten() {
    currentPacketSize++;
}

/**
 * @brief Clears all packet metadata from the packet framer
 */
void PacketFramer::clear() {
    currentPacketSize = 0;
    while (!availablePacketMetadata.empty()) {
        availablePacketMetadata.pop();
    }
}

/**
 * @brief Try getting the metadata of the latest packet received if a new one is available
 * @returns Metadata of the next packet if a packet is available, otherwise nullopt is returned
 */
std::optional<PacketMetadata> PacketFramer::tryGetNextPacketMetadata() {
    if (currentPacketSize < sizeof(TelemetryHeader)) {
        return std::nullopt;
    }

    getHeaderFromBuf(sizeof(TelemetryHeader));

    TelemetryHeader* header = reinterpret_cast<TelemetryHeader*>(headerBuf);
    bool isPacketValid = true;

    if (header->bits.type != TELEMETRY_TYPE_CODE && header->bits.type != STATUS_TYPE_CODE) {
        return std::nullopt;
    } else if (currentPacketSize > SerialConfig::MAX_PACKET_SIZE) {
        GCS_APP_LOG_WARN("PacketFramer: Packet of size ({}) bigger than max packet size ({}), ignoring packet.",
                         currentPacketSize,
                         SerialConfig::MAX_PACKET_SIZE);
        isPacketValid = false;
    }

    return PacketMetadata{.size = currentPacketSize, .isValid = isPacketValid, .packetTypeCode = header->bits.type, .boardId = header->bits.boardId};
}

/**
 * @brief Copies the header of packet written in the circular buffer to an internal buffer in the packet framer for further processing
 * @returns True if the header was successfully copied in the internal buffer, else false
 */
bool PacketFramer::getHeaderFromBuf(size_t headerSize) {
    if (headerSize > MAX_HEADER_SIZE) {
        GCS_APP_LOG_WARN(
          "PacketFramer: Tried to get header from circular buffer, but the header size ({}) is bigger than the maximum accepted header size ({}).",
          headerSize,
          MAX_HEADER_SIZE);
        return false;
    }

    if (headerSize > currentPacketSize) {
        GCS_APP_LOG_WARN(
          "PacketFramer: Tried to get header from circular buffer, but the header size ({}) is bigger than the size of the packet being currently "
          "read ({}).",
          headerSize,
          currentPacketSize);
        return false;
    }

    const size_t readAvailable = buf.readAvailable();
    if (headerSize > readAvailable) {
        GCS_APP_LOG_WARN("PacketFramer: Tried to get header from buffer, but the header size ({}) is bigger than the amount of read data available "
                         "in the circular buffer ({}).",
                         headerSize,
                         readAvailable);
        return false;
    }

    for (size_t i = 0; i < headerSize; i++) {
        std::optional<uint8_t> byte = buf.peekBack(headerSize - i - 1);
        if (!byte.has_value()) {
            GCS_APP_LOG_WARN("PacketFramer: Tried to get header from circular buffer, but not enough bytes are available.");
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
    return availablePacketMetadata.size();
}

/**
 * @brief Get the metadata of the next packet without removing it from the packet framer
 * @returns Metadata of the next packet
 */
std::optional<PacketMetadata> PacketFramer::peekNextPacketMetadata() const {
    if (!availablePacketMetadata.size()) {
        return std::nullopt;
    }
    return availablePacketMetadata.front();
}
