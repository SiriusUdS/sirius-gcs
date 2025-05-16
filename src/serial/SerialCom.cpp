#include "SerialCom.h"

#include "ComDiscovery.h"

/**
 * @brief Initializes communication on the first COM port found
 */
void SerialCom::start() {
    std::lock_guard<std::mutex> lock(mtx);
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
    std::lock_guard<std::mutex> lock(mtx);
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
    std::lock_guard<std::mutex> lock(mtx);
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
    std::lock_guard<std::mutex> lock(mtx);
    return com.IsOpened();
}

bool SerialCom::comWorking() {
    std::lock_guard<std::mutex> lock(mtx);
    return consecutiveFailedReads < Constants::SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE
           && consecutiveFailedWrites < Constants::SERIAL_MAX_CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE;
}

/**
 * @brief Fetches a packet from the internal buffer if one is available
 * @param recv The char buffer to receive the bytes into
 * @returns If a packet is found, the size of the packet is returned, otherwise 0 is returned
 */
size_t SerialCom::getPacket(uint8_t* recv) {
    std::lock_guard<std::mutex> lock(mtx);
    size_t packetSize = recvBuf.readPacket(recv);
    if (packetSize > 0) {
        packetsRead++;
    }
    return packetSize;
}

/**
 * @brief Returns the header code of the next available packet that can be read
 * @returns The next packet's header code if at least one packet is available, else 0
 */
uint32_t SerialCom::nextPacketHeaderCode() {
    std::lock_guard<std::mutex> lock(mtx);
    return recvBuf.nextPacketHeaderCode();
}

/**
 * @brief Returns the size of the next available packet that can be read
 * @returns The next packet's size if at least one packet is available, else 0
 */
size_t SerialCom::nextPacketSize() {
    std::lock_guard<std::mutex> lock(mtx);
    return recvBuf.nextPacketSize();
}

/**
 * @brief Dumps the next available packet.
 * @returns True if a packet was dumped, else false
 */
bool SerialCom::dumpNextPacket() {
    std::lock_guard<std::mutex> lock(mtx);
    return recvBuf.dumpNextPacket();
}

/**
 * @brief Returns the number of packets read per second
 * @returns Packets read per second
 */
size_t SerialCom::packetsReadPerSecond() {
    std::lock_guard<std::mutex> lock(mtx);
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - comStartTimePoint;
    return packetsRead / elapsedSeconds.count();
}

/**
 * @brief Shuts down communication with the currently opened serial COM port
 */
void SerialCom::shutdown() {
    std::lock_guard<std::mutex> lock(mtx);
    com.Close();
}
