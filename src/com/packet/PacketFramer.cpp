#include "PacketFramer.h"

#include "Logging.h"
#include "PacketCircularBuffer.h"
#include "Telecommunication/TelemetryHeader.h"

/**
 * @brief Constructs a packet framer by specifing the circular buffer to read the bytes from.
 */
PacketFramer::PacketFramer(const PacketCircularBuffer& buf) : buf{buf}, headerBuf{{0}} {
}

/**
 * @brief Try to frame a packet, starting from the last byte received in the circular buffer.
 * @returns Packet metadata if a packet was successfully framed, else std::nullopt.
 */
bool PacketFramer::tryFrame() {
    if (!checkForPacketStart()) {
        return false;
    }

    if (readingValidPacket) {
        currentPacketMetadata.status = PacketMetadata::Status::VALID;
    } else {
        readingValidPacket = true;
        currentPacketMetadata.status = PacketMetadata::Status::DUMP_IMMEDIATLY;
    }

    lastPacketMetadata = currentPacketMetadata;
    lastPacketMetadata->size -= sizeof(TelemetryHeader);

    return true;
}

/**
 * @brief Notify the packet framer that a byte was written in the circular buffer to keep track of the current packet's size.
 */
void PacketFramer::byteWritten() {
    currentPacketMetadata.size++;
}

void PacketFramer::newPacket() {
    currentPacketMetadata.status = PacketMetadata::Status::NONE;
    currentPacketMetadata.size = sizeof(TelemetryHeader);
}

/**
 * @brief Clears all packet framer state.
 */
void PacketFramer::clear() {
    currentPacketMetadata.status = PacketMetadata::Status::NONE;
    currentPacketMetadata.size = 0;
    lastPacketMetadata = std::nullopt;
    readingValidPacket = false;
}

std::optional<PacketMetadata> PacketFramer::getLastPacketMetadata() const {
    return lastPacketMetadata;
}

/**
 * @brief Check if a packet was received in the circular buffer, starting from the last byte written in the buffer.
 * @returns True if a packet was detected, else false.
 */
bool PacketFramer::checkForPacketStart() {
    if (currentPacketMetadata.size < sizeof(TelemetryHeader) || !getHeaderFromBuf(sizeof(TelemetryHeader))) {
        return false;
    }

    TelemetryHeader* header = (TelemetryHeader*) headerBuf;
    return header->bits.type == TELEMETRY_TYPE_CODE || header->bits.type == STATUS_TYPE_CODE;
}

/**
 * @brief Copies the header of packet written in the circular buffer to an internal buffer for further processing.
 * @returns True if the header was successfully copied into the internal buffer, else false.
 */
bool PacketFramer::getHeaderFromBuf(size_t headerSize) {
    if (headerSize > MAX_HEADER_SIZE) {
        GCS_APP_LOG_ERROR(
          "PacketFramer: Tried to get header from circular buffer, but the header of size {} is bigger than the maximum header size accepted of {}.",
          headerSize,
          MAX_HEADER_SIZE);
        return false;
    }

    if (headerSize > currentPacketMetadata.size) {
        GCS_APP_LOG_ERROR(
          "PacketFramer: Tried to get header from circular buffer, but the header of size {} is bigger than the packet being currently "
          "read of size {}.",
          headerSize,
          currentPacketMetadata.size);
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
