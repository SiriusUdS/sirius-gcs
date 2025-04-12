#ifndef RECVBUFFER_H
#define RECVBUFFER_H

#include "Telecommunication/TelecommunicationHeader.h"
#include "WordFormatter.h"

#include <optional>
#include <queue>

template <size_t BUFSIZE>
class RecvBuffer {
public:
    size_t readPacket(uint8_t* recv);
    bool writeChar(uint8_t c);
    size_t availablePackets();
    bool isFull();
    void clear();

private:
    std::optional<uint32_t> searchAnyHeader(size_t idx);
    bool searchSpecificHeader(uint32_t headerCode, size_t idx);

    size_t nextIndex(size_t idx, size_t increment = 1);
    size_t prevIndex(size_t idx, size_t decrement = 1);

    size_t readIdx{};
    size_t writeIdx{};
    uint8_t buf[BUFSIZE]{};
    bool bufFull{};
    bool writingValidPacket{};
    size_t currPacketSize{};
    std::queue<size_t> availablePacketSizeQueue{};
};

template <size_t BUFSIZE>
size_t RecvBuffer<BUFSIZE>::readPacket(uint8_t* recv) {
    if (availablePacketSizeQueue.size() == 0) {
        return 0;
    }

    size_t packetSize = availablePacketSizeQueue.front();
    availablePacketSizeQueue.pop();

    for (size_t idx = 0; idx < packetSize; idx++) {
        recv[idx] = buf[readIdx];
        readIdx = nextIndex(readIdx);
    }

    bufFull = false;
    return packetSize;
}

template <size_t BUFSIZE>
bool RecvBuffer<BUFSIZE>::writeChar(uint8_t c) {
    if (bufFull) {
        return false;
    }

    buf[writeIdx] = c;
    writeIdx = nextIndex(writeIdx);
    currPacketSize++;
    bufFull = (writeIdx == readIdx);

    if (currPacketSize < HEADER_SIZE_BYTE) {
        return true;
    }

    if (currPacketSize >= HEADER_SIZE_BYTE) {
        std::optional<uint32_t> optionalHeaderCode = searchAnyHeader(prevIndex(writeIdx, HEADER_SIZE_BYTE));
        if (optionalHeaderCode.has_value()) {
            if (!writingValidPacket) {
                writingValidPacket = true;
            } else {
                availablePacketSizeQueue.push(currPacketSize);
            }
            currPacketSize = 0;
        }
    }

    return true;
}

template <size_t BUFSIZE>
size_t RecvBuffer<BUFSIZE>::availablePackets() {
    return availablePacketSizeQueue.size();
}

template <size_t BUFSIZE>
inline bool RecvBuffer<BUFSIZE>::isFull() {
    return bufFull;
}

template <size_t BUFSIZE>
inline void RecvBuffer<BUFSIZE>::clear() {
    readIdx = 0;
    writeIdx = 0;
    bufFull = false;
    writingValidPacket = false;
    currPacketSize = 0;
    while (!availablePacketSizeQueue.empty()) {
        availablePacketSizeQueue.pop();
    }
}

template <size_t BUFSIZE>
std::optional<uint32_t> RecvBuffer<BUFSIZE>::searchAnyHeader(size_t idx) {
    // clang-format off
    static const uint32_t HEADER_CODES[9] = {
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

    for (const uint32_t headerCode : HEADER_CODES) {
        if (searchSpecificHeader(headerCode, idx)) {
            return headerCode;
        }
    }

    return {};
}

template <size_t BUFSIZE>
bool RecvBuffer<BUFSIZE>::searchSpecificHeader(uint32_t headerCode, size_t idx) {
    // The header code is received in reversed (little endian), so it needs to be flipped (Ex. Received as \0MHT, convert to THM\0)
    uint32_t headerCodeAtIdx = (buf[nextIndex(idx, 1)] << 8) | (buf[nextIndex(idx, 2)] << 16) | (buf[nextIndex(idx, 3)] << 24);
    return headerCode == headerCodeAtIdx;
}

template <size_t BUFSIZE>
size_t RecvBuffer<BUFSIZE>::nextIndex(size_t idx, size_t increment) {
    return (idx + increment) % BUFSIZE;
}

template <size_t BUFSIZE>
size_t RecvBuffer<BUFSIZE>::prevIndex(size_t idx, size_t decrement) {
    decrement %= BUFSIZE;
    if (decrement > idx) {
        idx += BUFSIZE;
    }
    return idx - decrement;
}

#endif // RECVBUFFER_H
