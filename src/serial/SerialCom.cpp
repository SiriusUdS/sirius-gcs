#include "SerialCom.h"

#include "ComDiscovery.h"

/**
 * @brief Initializes communication on the first COM port found
 */
void SerialCom::start() {
    com.Close();
    packetsRead = 0;
    consecutiveFailedReads = 0;
    consecutiveFailedWrites = 0;

    std::vector<std::string> availableComPorts;
    getAvailableComPorts(availableComPorts);
    if (availableComPorts.empty()) {
        return;
    }

    std::string comPath = std::string("\\\\.\\") + availableComPorts[0];
    com.SetPortName(comPath);
    com.Open();
    comStartTimePoint = std::chrono::steady_clock::now();
}

/**
 * @brief Reads a single byte from the COM port into an internal buffer
 * @returns True if a byte was successfully read, else false
 */
bool SerialCom::read() {
    if (!com.IsOpened()) {
        return false;
    }

    bool successFlag;
    char c = com.ReadChar(successFlag);
    if (successFlag) {
        consecutiveFailedReads = 0;
        recvBuf.writeChar(c);
    } else {
        consecutiveFailedReads++;
    }
    return successFlag;
}

/**
 * @brief Send data through the opened COM port
 * @param msg The char buffer that contains the data to send
 * @param size The size of the data to send
 * @returns True if the data was successfully sent, else false
 */
bool SerialCom::write(uint8_t* msg, size_t size) {
    if (!com.IsOpened()) {
        return false;
    }

    bool successfulFlag = com.Write((char*) msg, (long) size);
    if (successfulFlag) {
        consecutiveFailedWrites = 0;
    } else {
        consecutiveFailedWrites++;
    }
    return successfulFlag;
}

/**
 * @brief Checks if a COM port is opened
 * @returns True if a COM port is opened, else false
 */
bool SerialCom::comOpened() {
    return com.IsOpened();
}

bool SerialCom::comWorking() {
    return consecutiveFailedReads < Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE
           && consecutiveFailedWrites < Constants::SERIAL_MAX_CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE;
}

/**
 * @brief Fetches a packet from the internal buffer if one is available
 * @param recv The char buffer to receive the bytes into
 * @returns If a packet is found, the size of the packet is returned, otherwise 0 is returned
 */
size_t SerialCom::getPacket(uint8_t* recv) {
    size_t packetSize = recvBuf.readPacket(recv);
    if (packetSize > 0) {
        packetsRead++;
    }
    return packetSize;
}

uint32_t SerialCom::nextPacketHeaderCode() {
    return recvBuf.nextPacketHeaderCode();
}

size_t SerialCom::nextPacketSize() {
    return recvBuf.nextPacketSize();
}

bool SerialCom::dumpNextPacket() {
    return recvBuf.dumpNextPacket();
}

size_t SerialCom::packetsReadPerSecond() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - comStartTimePoint;
    return packetsRead / elapsedSeconds.count();
}

/**
 * @brief Shuts down communication with the currently opened serial COM port
 */
void SerialCom::shutdown() {
    com.Close();
}
