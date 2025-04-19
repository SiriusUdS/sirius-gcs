#include "SerialCom.h"

#include "ComDiscovery.h"

/**
 * @brief Initializes communication on the first COM port found
 */
void SerialCom::start() {
    if (com.IsOpened()) {
        com.Close();
    }

    std::vector<std::string> availableComPorts;
    getAvailableComPorts(availableComPorts);
    if (availableComPorts.empty()) {
        return;
    }

    std::string comPath = std::string("\\\\.\\") + availableComPorts[0];
    com.SetPortName(comPath);
    com.Open();
}

/**
 * @brief Checks if a COM port is opened
 * @returns True if a COM port is opened, else false
 */
bool SerialCom::comOpened() {
    return com.IsOpened();
}

/**
 * @brief Reads a single byte from the COM port into an internal buffer
 */
void SerialCom::read() {
    if (!com.IsOpened()) {
        return;
    }

    bool successFlag;
    char c = com.ReadChar(successFlag);
    if (successFlag) {
        recvBuf.writeChar(c);
    } else {
        GCS_LOG_TRACE("SerialCom: No available bytes in recv COM port.");
    }
}

/**
 * @brief Fetches a packet from the internal buffer if one is available
 * @param recv The char buffer to receive the bytes into
 * @returns If a packet is found, the size of the packet is returned, otherwise 0 is returned
 */
size_t SerialCom::getPacket(uint8_t* recv) {
    return recvBuf.readPacket(recv);
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

    return com.Write((char*) msg, (long) size);
}

/**
 * @brief Shuts down communication with the currently opened serial COM port
 */
void SerialCom::shutdown() {
    com.Close();
}
