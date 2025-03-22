#include "MsgBuffer.h"

#include "Telecommunication/TelecommunicationHeader.h"

size_t MsgBuffer::readPacket(char* rcv) {
    if (nbPacketsReady == 0) {
        return 0;
    }

    for (int idx = 0; idx < currPacketSize; idx++) {
        rcv[idx] = buf[readIdx];
        advanceIndex(readIdx);
    }

    nbPacketsReady--;
    size_t packetSize = currPacketSize;
    currPacketSize = 0;

    return packetSize;
}

bool MsgBuffer::writeChar(char c) {
    if (!canWrite(1)) {
        return false;
    }

    buf[writeIdx] = c;
    writeIdx = nextIndex(writeIdx);
    if (readCapacity() >= HEADER_SIZE_BYTE) {
        std::optional<std::pair<int, size_t>> optionalHeader = searchAnyHeader(prevIndex(writeIdx, 4));
        if (optionalHeader.has_value()) {
            nbPacketsReady++;
        }
    }

    bufFull = (writeIdx == readIdx);

    return true;
}

bool MsgBuffer::canRead(size_t size) {
    return nbPacketsReady > 0;
}

bool MsgBuffer::canWrite(size_t size) {
    return writeCapacity() >= size;
}

int MsgBuffer::availablePackets() {
    return nbPacketsReady;
}

std::optional<std::pair<int, size_t>> MsgBuffer::searchAnyHeader(size_t idx) {
    // clang-format off
    static const int HEADER_CODES[9] = {
        ACCELEROMETER_DATA_HEADER_CODE,
        GYROSCOPE_DATA_HEADER_CODE,
        ALTIMETER_DATA_HEADER_CODE,
        GPS_DATA_HEADER_CODE,
        MAGNETOMETER_DATA_HEADER_CODE,
        PRESSURE_SENSOR_DATA_HEADER_CODE,
        ROCKET_DATA_HEADER_CODE,
        TEMPERATURE_SENSOR_DATA_HEADER_CODE,
        VALVE_DATA_HEADER_CODE
    };
    // clang-format on

    for (const int headerCode : HEADER_CODES) {
        const std::optional<size_t> optionalIdx = searchHeader(headerCode, idx);
        if (optionalIdx.has_value()) {
            return std::pair<int, size_t>{headerCode, optionalIdx.value()};
        }
    }

    return {};
}

std::optional<size_t> MsgBuffer::searchHeader(int headerCode, size_t idx) {
    // clang-format off
    char headerStr[4] = {
        (char) (headerCode >> 24),
        (char) ((headerCode >> 16) & ((1 << 16) - 1)),
        (char) ((headerCode >> 8) & ((1 << 16) - 1)),
        (char) ((headerCode) & ((1 << 16) - 1))
    };
    
    if (headerStr[0] == buf[idx] &&
        headerStr[1] == buf[nextIndex(idx)] &&
        headerStr[2] == buf[nextIndex(idx, 2)] &&
        headerStr[3] == buf[nextIndex(idx, 3)]
    ) {
        return idx;
    }

    return {};
    // clang-format on
}

size_t MsgBuffer::readCapacity() {
    return readCapacityFromIdx(readIdx);
}

size_t MsgBuffer::writeCapacity() {
    return writeCapacityFromIdx(writeIdx);
}

size_t MsgBuffer::readCapacityFromIdx(size_t idx) {
    if (idx < writeIdx) {
        return (writeIdx - idx);
    } else if (idx > writeIdx) {
        return (writeIdx + Constants::MSG_BUF_SIZE - idx);
    } else if (bufFull) {
        return Constants::MSG_BUF_SIZE;
    }
    return 0;
}

size_t MsgBuffer::writeCapacityFromIdx(size_t idx) {
    if (idx > readIdx) {
        return (readIdx + Constants::MSG_BUF_SIZE - idx);
    } else if (idx < readIdx) {
        return readIdx - idx;
    } else if (bufFull) {
        return 0;
    }
    return Constants::MSG_BUF_SIZE;
}

size_t MsgBuffer::nextIndex(size_t idx, size_t increment) {
    return (idx + increment) % Constants::MSG_BUF_SIZE;
}

size_t MsgBuffer::prevIndex(size_t idx, size_t decrement) {
    idx -= decrement;
    while (idx < 0) {
        idx += Constants::MSG_BUF_SIZE;
    }
    return idx;
}

void MsgBuffer::advanceIndex(size_t& idx, size_t increment) {
    idx = nextIndex(idx, increment);
}
