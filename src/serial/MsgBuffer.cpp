#include "MsgBuffer.h"

#include "Telecommunication/TelecommunicationHeader.h"

size_t MsgBuffer::readPacket(char* rcv) {
    if (!canReadPacket()) {
        return 0;
    }

    PacketInfo& packetInfo = availablePacketInfoQueue.back();

    for (int idx = 0; idx < packetInfo.size; idx++) {
        rcv[idx] = buf[readIdx];
        readIdx = nextIndex(readIdx);
    }

    size_t packetSize = packetInfo.size;
    availablePacketInfoQueue.pop();
    return packetSize;
}

bool MsgBuffer::writeChar(char c) {
    if (!canWriteChar()) {
        return false;
    }

    buf[writeIdx] = c;
    writeIdx = nextIndex(writeIdx);
    currPacket.size++;
    bufFull = (writeIdx == readIdx);

    if (readCapacity() >= HEADER_SIZE_BYTE) {
        std::optional<int> optionalHeaderCode = searchAnyHeader(prevIndex(writeIdx, 4));
        if (optionalHeaderCode.has_value()) {
            if (!writingValidPacket) {
                writingValidPacket = true;
            } else {
                availablePacketInfoQueue.push(currPacket);
                currPacket.headerCode = optionalHeaderCode.value();
                currPacket.size = 0;
            }
        }
    }

    return true;
}

bool MsgBuffer::canReadPacket() {
    return availablePacketInfoQueue.size() > 0;
}

bool MsgBuffer::canWriteChar() {
    return writeCapacity() >= 1;
}

int MsgBuffer::availablePackets() {
    return availablePacketInfoQueue.size();
}

std::optional<int> MsgBuffer::searchAnyHeader(size_t idx) {
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
        if (searchSpecificHeader(headerCode, idx)) {
            return headerCode;
        }
    }

    return {};
}

bool MsgBuffer::searchSpecificHeader(int headerCode, size_t idx) {
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
        return true;
    }

    return false;
    // clang-format on
}

size_t MsgBuffer::readCapacity() {
    if (readIdx < writeIdx) {
        return (writeIdx - readIdx);
    } else if (readIdx > writeIdx) {
        return (writeIdx + Constants::MSG_BUF_SIZE - readIdx);
    } else if (bufFull) {
        return Constants::MSG_BUF_SIZE;
    }
    return 0;
}

size_t MsgBuffer::writeCapacity() {
    if (writeIdx > readIdx) {
        return (readIdx + Constants::MSG_BUF_SIZE - writeIdx);
    } else if (writeIdx < readIdx) {
        return readIdx - writeIdx;
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
