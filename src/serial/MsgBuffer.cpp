#include "MsgBuffer.h"

#include "Telecommunication/TelecommunicationHeader.h"

bool MsgBuffer::read(char* rcv, size_t size) {
    if (!canRead(size)) {
        return false;
    }

    for (size_t rcvIdx = 0; rcvIdx < size; rcvIdx++) {
        readIdx = (readIdx + 1) % bufSize;
        rcv[rcvIdx] = buf[readIdx];
    }

    return true;
}

bool MsgBuffer::write(char* msg, size_t size) {
    if (!canWrite(size)) {
        return false;
    }

    size_t oldWriteIdx = writeIdx;

    for (size_t charIdx = 0; charIdx < size; charIdx++) {
        writeIdx = (writeIdx + 1) % bufSize;
        buf[writeIdx] = msg[charIdx];
    }

    bufFull = (writeIdx == readIdx);

    // Compute buffer index at which to start looking for the header of the next packet in order
    // to check if the current packet has been fully received.
    size_t headerStartReadIdx = oldWriteIdx - (HEADER_SIZE_BYTE - 1);
    if (headerStartReadIdx < 0) {
        headerStartReadIdx += bufSize;
    }
    if (readCapacityFromIdx(oldWriteIdx) < readCapacityFromIdx(headerStartReadIdx)) {
        headerStartReadIdx = readIdx;
    }

    return true;
}

bool MsgBuffer::canRead(size_t size) {
    return readCapacity() >= size;
}

bool MsgBuffer::canWrite(size_t size) {
    return writeCapacity() >= size;
}

size_t MsgBuffer::readCapacity() {
    return readCapacityFromIdx(readIdx);
}

size_t MsgBuffer::writeCapacity() {
    return writeCapacityFromIdx(writeIdx);
}

std::optional<size_t> MsgBuffer::searchAnyHeader(size_t startIdx, size_t endIdx) {
    // TODO - Finish optional chaining
    searchHeader(ACCELEROMETER_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(GYROSCOPE_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(ALTIMETER_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(GPS_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(MAGNETOMETER_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(PRESSURE_SENSOR_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(ROCKET_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(TEMPERATURE_SENSOR_DATA_HEADER_CODE, startIdx, endIdx);
    searchHeader(VALVE_DATA_HEADER_CODE, startIdx, endIdx);
}

std::optional<size_t> MsgBuffer::searchHeader(int headerCode, size_t startIdx, size_t endIdx) {
    // clang-format off
    char headerStr[4] = {
        (char) (headerCode >> 48),
        (char) ((headerCode >> 32) & ((1 << 16) - 1)),
        (char) ((headerCode >> 16) & ((1 << 16) - 1)),
        (char) ((headerCode) & ((1 << 16) - 1))
    };
    
    endIdx -= HEADER_SIZE_BYTE;
    if (endIdx < 0) {
        endIdx += bufSize;
    }
    for (size_t idx = startIdx; idx != endIdx; ++idx % bufSize) {
        if (headerStr[0] == buf[idx] &&
            headerStr[1] == buf[idx + 1] &&
            headerStr[2] == buf[idx + 2] &&
            headerStr[3] == buf[idx + 3]
        ) {
            return idx;
        }
    }

    return {};
    // clang-format on
}

size_t MsgBuffer::readCapacityFromIdx(size_t idx) {
    if (idx < writeIdx) {
        return (writeIdx - idx);
    } else if (idx > writeIdx) {
        return (writeIdx + bufSize - idx);
    } else if (bufFull) {
        return bufSize;
    }
    return 0;
}

size_t MsgBuffer::writeCapacityFromIdx(size_t idx) {
    if (idx > readIdx) {
        return (readIdx + bufSize - idx);
    } else if (idx < readIdx) {
        return readIdx - idx;
    } else if (bufFull) {
        return 0;
    }
    return bufFull;
}
