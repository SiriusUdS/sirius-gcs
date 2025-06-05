#include "PacketFramer.h"

#include "CircularBuffer.hpp"
#include "Logging.h"
#include "Telecommunication/TelemetryHeader.h"

PacketFramer::PacketFramer(const CircularBuffer<Constants::RECV_BUF_SIZE>& buf) : buf{buf}, headerBuf{{0}} {
}

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

size_t PacketFramer::consumeNextPacketSize() {
    if (!availablePacketSizesQueue.size()) {
        GCS_LOG_WARN("PacketFramer: Tried to consume next packet size, but no packets available.");
        return 0;
    }

    size_t size = availablePacketSizesQueue.front();
    availablePacketSizesQueue.pop();
    return size;
}

void PacketFramer::byteWritten() {
    currentPacketSize++;
}

void PacketFramer::clear() {
    currentPacketSize = 0;
    while (!availablePacketSizesQueue.empty()) {
        availablePacketSizesQueue.pop();
    }
}

bool PacketFramer::checkForPacketStart() {
    return checkForTelemetryPacketStart();

    // TODO: Check for GS Control packet
}

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

bool PacketFramer::getHeaderFromBuf(size_t headerSize) {
    if (headerSize > Constants::RECV_PACKET_MAX_HEADER_SIZE) {
        GCS_LOG_WARN(
          "PacketFramer: Tried to get header from circular buffer, but the header of size {} is bigger than the maximum header size accepted of {}.",
          headerSize, Constants::RECV_PACKET_MAX_HEADER_SIZE);
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

bool PacketFramer::packetAvailable() const {
    return availablePacketSizesQueue.size();
}

size_t PacketFramer::peekNextPacketSize() const {
    if (!availablePacketSizesQueue.size()) {
        return 0;
    }
    return availablePacketSizesQueue.front();
}
