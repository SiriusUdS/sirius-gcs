#ifndef RECVBUFFER_H
#define RECVBUFFER_H

#include "Telecommunication/TelecommunicationHeader.h"

#include <optional>
#include <queue>

template <size_t BUFSIZE>
class RecvBuffer {
public:
    size_t readPacket(char* recv);
    bool writeChar(char c);
    size_t availablePackets();
    bool isBufferFull();
    void clear();

private:
    std::optional<int> searchAnyHeader(size_t idx);
    bool searchSpecificHeader(int headerCode, size_t idx);

    size_t nextIndex(size_t idx, size_t increment = 1);
    size_t prevIndex(size_t idx, size_t decrement = 1);

    size_t readIdx{};
    size_t writeIdx{};
    char buf[BUFSIZE]{};
    bool bufFull{};
    bool writingValidPacket{};
    size_t currPacketSize{};
    std::queue<size_t> availablePacketSizeQueue{};
};

template <size_t BUFSIZE>
size_t RecvBuffer<BUFSIZE>::readPacket(char* recv) {
    if (availablePacketSizeQueue.size() == 0) {
        return 0;
    }

    size_t packetSize = availablePacketSizeQueue.front();
    availablePacketSizeQueue.pop();

    for (int idx = 0; idx < packetSize; idx++) {
        recv[idx] = buf[readIdx];
        readIdx = nextIndex(readIdx);
    }

    bufFull = false;
    return packetSize;
}

template <size_t BUFSIZE>
bool RecvBuffer<BUFSIZE>::writeChar(char c) {
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
        std::optional<int> optionalHeaderCode = searchAnyHeader(prevIndex(writeIdx, 4));
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
inline bool RecvBuffer<BUFSIZE>::isBufferFull() {
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
std::optional<int> RecvBuffer<BUFSIZE>::searchAnyHeader(size_t idx) {
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
bool RecvBuffer<BUFSIZE>::searchSpecificHeader(int headerCode, size_t idx) {
    int headerCodeAtIdx = (buf[idx] << 24) | (buf[nextIndex(idx)] << 16) | (buf[nextIndex(idx, 2)] << 8);
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
