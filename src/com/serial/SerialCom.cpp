#include "SerialCom.h"

#include "ComDiscovery.h"
#include "PacketRateMonitor.h"
#include "PacketReceiver.h"
#include "SerialStateMonitor.h"

/**
 * @brief Constructs a SerialCom object with the given monitors.
 */
SerialCom::SerialCom(PacketRateMonitor& prm, PacketReceiver& pr, SerialStateMonitor& sfm)
    : packetRateMonitor(prm), packetReceiver(pr), serialFailureMonitor(sfm) {
}

/**
 * @brief Initializes communication on the first COM port found.
 */
void SerialCom::start() {
    com.Close();
    packetRateMonitor.reset();
    serialFailureMonitor.reset();

    std::vector<std::string> availableComPorts;
    ComDiscovery::getAvailableComPorts(availableComPorts);
    if (availableComPorts.empty()) {
        return;
    }

    std::string comPath = std::string("\\\\.\\") + availableComPorts[0];
    com.SetPortName(comPath);
    com.SetBaudRate(CBR_19200);
    com.Open();
}

/**
 * @brief Reads a single byte from the COM port into an internal buffer.
 * @returns True if a byte was successfully read, otherwise false.
 */
bool SerialCom::read() {
    bool successful;
    char c = com.ReadChar(successful);
    if (successful) {
        successful = packetReceiver.receiveByte(c);
    }
    serialFailureMonitor.trackRead(successful);
    return successful;
}

/**
 * @brief Send data through the opened COM port.
 * @param msg The char buffer that contains the data to send.
 * @param size The size of the data to send.
 * @returns True if the data was successfully sent, otherwise false.
 */
bool SerialCom::write(uint8_t* msg, size_t size) {
    static constexpr size_t WRITE_TIMEOUT_MS = 100;
    bool successful = com.WriteArr(msg, (long) size, WRITE_TIMEOUT_MS);
    serialFailureMonitor.trackWrite(successful);
    return successful;
}

/**
 * @brief Checks if a COM port is opened.
 * @returns True if a COM port is opened, otherwise false.
 */
bool SerialCom::comOpened() {
    return com.IsOpened();
}

/**
 * @brief Fetches a packet from the internal buffer if one is available.
 * @param recv The char buffer to receive the bytes into.
 * @returns True if a packet was successfully received, otherwise false.
 */
bool SerialCom::getPacket(uint8_t* recv) {
    bool successful = packetReceiver.getPacket(recv);
    if (successful) {
        packetRateMonitor.trackPacket();
    }
    return successful;
}

/**
 * @brief Gets a pointer to the internal buffer used for receiving data.
 * @returns Pointer to the internal buffer.
 */
uint8_t* SerialCom::getBuffer() {
    return packetReceiver.getBuffer();
}

/**
 * @brief Shuts down communication with the currently opened serial COM port.
 */
void SerialCom::shutdown() {
    com.Close();
}
