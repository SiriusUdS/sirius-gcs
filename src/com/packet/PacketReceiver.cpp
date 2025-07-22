#include "PacketReceiver.h"

#include "Logging.h"

/**
 * @brief Receive a single byte. Detects if a packet has been received every time a byte gets processed.
 * @param byte Byte to receive
 * @returns True if packet successfully received, else false
 */
bool PacketReceiver::receiveByte(uint8_t byte) {
    if (buf.isFull() && !pf.packetAvailable()) {
        GCS_APP_LOG_WARN("PacketReceiver: Circular buffer full, but no packet available. Clearing buffer.");
        clear();
    }

    if (!buf.writeByte(byte)) {
        GCS_APP_LOG_WARN("PacketReceiver: Unable to write byte in circular buffer.");
        return false;
    }

    pf.byteWritten();
    pf.tryFrame();

    return true;
}

/**
 * @brief Gets a previously received packet and copies its contents into a byte buffer
 * @param recv The buffer in which the packet's data will be written in
 * @returns True if the packet was successfully written in the reception buffer, else false
 */
bool PacketReceiver::getPacket(uint8_t* recv) {
    if (!pf.packetAvailable()) {
        return false;
    }

    size_t size = pf.consumeNextPacketSize();
    if (!buf.read(recv, size)) {
        GCS_APP_LOG_ERROR("PacketReceiver: Tried reading data from the circular buffer, but not enough data is available. Clearing all data to avoid "
                          "desynchronization.");
        clear();
        return false;
    }

    return true;
}

/**
 * @brief Gets a pointer to the internal buffer used for receiving data.
 * @returns Pointer to the internal buffer.
 */
uint8_t* PacketReceiver::getBuffer() {
    return buf.getBuffer();
}

/**
 * @brief Dumps the next packet, ignoring it completely
 * @returns True if a packet was successfully dumped, else false
 */
bool PacketReceiver::dumpNextPacket() {
    size_t size = pf.consumeNextPacketSize();

    if (size == 0) {
        GCS_APP_LOG_WARN("PacketReceiver: Tried to dump next packet, but no packet available.");
        return false;
    }

    if (!buf.dump(size)) {
        GCS_APP_LOG_WARN("PacketReceiver: Tried to dump next packet, but circular buffer did not have enough read data left.");
        return false;
    }

    return true;
}

/**
 * @brief Returns the next packet's size
 * @returns Next packet's size
 */
size_t PacketReceiver::nextPacketSize() const {
    return pf.peekNextPacketSize();
}

/**
 * @brief Clears the packet framer and circular buffer.
 */
void PacketReceiver::clear() {
    buf.clear();
    pf.clear();
}
