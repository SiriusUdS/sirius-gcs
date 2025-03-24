#ifndef MSGBUFFER_H
#define MSGBUFFER_H

#include "Telecommunication/TelecommunicationHeader.h"

#include <optional>
#include <queue>

template <size_t BUFSIZE>
class MsgBuffer {
public:
    size_t readPacket(int* headerCode, char* rcv);
    bool writeChar(char c);
    bool canReadPacket();
    bool canWriteChar();
    size_t availablePackets();

private:
    struct PacketInfo {
        int headerCode;
        size_t size;
    };

    std::optional<int> searchAnyHeader(size_t idx);
    bool searchSpecificHeader(int headerCode, size_t idx);

    size_t readCapacity();
    size_t writeCapacity();

    size_t nextIndex(size_t idx, size_t increment = 1);
    size_t prevIndex(size_t idx, size_t decrement = 1);

    size_t readIdx{};
    size_t writeIdx{};
    char buf[BUFSIZE]{};
    bool bufFull{};
    bool writingValidPacket{};
    PacketInfo currPacket{};
    std::queue<PacketInfo> availablePacketInfoQueue{};
};

template <size_t BUFSIZE>
size_t MsgBuffer<BUFSIZE>::readPacket(int* headerCode, char* rcv) {
    if (!canReadPacket()) {
        return 0;
    }

    PacketInfo& packetInfo = availablePacketInfoQueue.front();
    size_t dataSize = packetInfo.size - HEADER_SIZE_BYTE;

    *headerCode = packetInfo.headerCode;

    readIdx = nextIndex(readIdx, HEADER_SIZE_BYTE);
    for (int idx = 0; idx < dataSize; idx++) {
        rcv[idx] = buf[readIdx];
        readIdx = nextIndex(readIdx);
    }

    availablePacketInfoQueue.pop();
    return dataSize;
}

template <size_t BUFSIZE>
bool MsgBuffer<BUFSIZE>::writeChar(char c) {
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
            }
            currPacket.headerCode = optionalHeaderCode.value();
            currPacket.size = 0;
        }
    }

    return true;
}

template <size_t BUFSIZE>
bool MsgBuffer<BUFSIZE>::canReadPacket() {
    return availablePacketInfoQueue.size() > 0;
}

template <size_t BUFSIZE>
bool MsgBuffer<BUFSIZE>::canWriteChar() {
    return writeCapacity() >= 1;
}

template <size_t BUFSIZE>
size_t MsgBuffer<BUFSIZE>::availablePackets() {
    return availablePacketInfoQueue.size();
}

template <size_t BUFSIZE>
std::optional<int> MsgBuffer<BUFSIZE>::searchAnyHeader(size_t idx) {
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

template <size_t BUFSIZE>
bool MsgBuffer<BUFSIZE>::searchSpecificHeader(int headerCode, size_t idx) {
    int headerCodeAtIdx = (buf[idx] << 24) | (buf[nextIndex(idx)] << 16) | (buf[nextIndex(idx, 2)] << 8) | buf[nextIndex(idx, 3)];
    return headerCode == headerCodeAtIdx;
}

template <size_t BUFSIZE>
size_t MsgBuffer<BUFSIZE>::readCapacity() {
    if (readIdx < writeIdx) {
        return (writeIdx - readIdx);
    } else if (readIdx > writeIdx) {
        return (writeIdx + BUFSIZE - readIdx);
    } else if (bufFull) {
        return BUFSIZE;
    }
    return 0;
}

template <size_t BUFSIZE>
size_t MsgBuffer<BUFSIZE>::writeCapacity() {
    if (writeIdx > readIdx) {
        return (readIdx + BUFSIZE - writeIdx);
    } else if (writeIdx < readIdx) {
        return readIdx - writeIdx;
    } else if (bufFull) {
        return 0;
    }
    return BUFSIZE;
}

template <size_t BUFSIZE>
size_t MsgBuffer<BUFSIZE>::nextIndex(size_t idx, size_t increment) {
    return (idx + increment) % BUFSIZE;
}

template <size_t BUFSIZE>
size_t MsgBuffer<BUFSIZE>::prevIndex(size_t idx, size_t decrement) {
    decrement %= BUFSIZE;
    if (decrement > idx) {
        idx += BUFSIZE;
    }
    return idx - decrement;
}

#endif // MSGBUFFER_H
