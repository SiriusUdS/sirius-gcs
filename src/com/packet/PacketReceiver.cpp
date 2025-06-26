#include "PacketReceiver.h"

#include "Logging.h"

/**
 * @brief Receive a single byte. Detects if a packet has been received every time a byte gets processed.
 * @param byte Byte to receive
 * @returns True if packet successfully received, else false
 */
bool PacketReceiver::receiveByte(uint8_t byte) {
    if (!buf.writeByte(byte)) {
        GCS_LOG_WARN("PacketReceiver: Unable to write byte in circular buffer.");
        return false;
    }

    if (buf.isFull() && !pf.packetAvailable()) {
        GCS_LOG_WARN("PacketReceiver: Circular buffer full, but no packet available. Clearing buffer.");
        buf.clear();
        pf.clear();
        return true;
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
    buf.read(recv, size);

    return true;
}

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
        GCS_LOG_WARN("PacketReceiver: Tried to dump next packet, but no packet available.");
        return false;
    }

    if (!buf.dump(size)) {
        GCS_LOG_WARN("PacketReceiver: Tried to dump next packet, but circular buffer did not have enough read data left.");
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
