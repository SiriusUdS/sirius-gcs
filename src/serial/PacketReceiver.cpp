#include "PacketReceiver.h"

#include "Logging.h"

bool PacketReceiver::receiveByte(uint8_t c) {
    if (!buf.writeByte(c)) {
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

bool PacketReceiver::getPacket(uint8_t* recv) {
    if (!pf.packetAvailable()) {
        return false;
    }

    size_t size = pf.consumeNextPacketSize();
    buf.read(recv, size);

    return true;
}

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

size_t PacketReceiver::nextPacketSize() const {
    return pf.peekNextPacketSize();
}
