#include "PacketFramer.h"

#include "Logging.h"
#include "Telecommunication/TelemetryHeader.h"

PacketFramer::PacketFramer(CircularBuffer& buf) : buf{buf} {
}

std::optional<uint32_t> PacketFramer::checkForPacketStart() {
    static const uint32_t TELEMETRY_HEADER_CODES[] = {TELEMETRY_TYPE_CODE, STATUS_TYPE_CODE};

    for (const uint32_t headerCode : TELEMETRY_HEADER_CODES) {
        TelemetryHeader header;
        header.value = (buf[nextIndex(idx, 3)] << 24) | (buf[nextIndex(idx, 2)] << 16) | (buf[nextIndex(idx, 1)] << 8) | buf[idx];
        if (headerCode == header.bits.type) {
        }
    }
    return {};
}

bool PacketFramer::getHeaderFromBuf(size_t headerSize) {
    if (headerSize > Constants::RECV_PACKET_MAX_HEADER_SIZE) {
        GCS_LOG_WARN("PacketFramer: Header size is bigger that the maximum header size accepted.");
        return false;
    }

    for (size_t i = 0; i < headerSize; i++) {
    }

    return true;
}
