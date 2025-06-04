#include "CircularBuffer.hpp"
#include "PacketFramer.h"
#include "Telecommunication/TelemetryHeader.h"
#include "Telecommunication/TelemetryPacket.h"

#include <doctest.h>

void writeTelemetryPacket(CircularBuffer<>& buf, PacketFramer& pf) {
    EngineTelemetryPacket packet;
    packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;

    for (size_t i = 0; i < sizeof(EngineTelemetryPacket); i++) {
        uint8_t byte = packet.data[i];
        buf.writeChar(byte);
        pf.byteWritten();
        pf.tryFrame();
    }
}

void fillCircularBuffer(CircularBuffer<>& buf, PacketFramer& pf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf.writeChar('0');
        pf.byteWritten();
        pf.tryFrame();
    }
}

TEST_CASE("Detect packets in packet framer") {
    CircularBuffer buf;
    PacketFramer pf(buf);

    CHECK_FALSE(pf.packetAvailable());
    writeTelemetryPacket(buf, pf);
    CHECK_FALSE(pf.packetAvailable());
    writeTelemetryPacket(buf, pf);
    CHECK(pf.packetAvailable());
}

TEST_CASE("Consume next packet size in packet framer") {
    CircularBuffer buf;
    PacketFramer pf(buf);

    CHECK(pf.consumeNextPacketSize() == 0);
    writeTelemetryPacket(buf, pf);
    CHECK(pf.consumeNextPacketSize() == 0);
    writeTelemetryPacket(buf, pf);
    fillCircularBuffer(buf, pf, 4);
    CHECK(pf.consumeNextPacketSize() == sizeof(EngineTelemetryPacket));
    writeTelemetryPacket(buf, pf);
    fillCircularBuffer(buf, pf, 8);
    writeTelemetryPacket(buf, pf);
    fillCircularBuffer(buf, pf, 100);
    writeTelemetryPacket(buf, pf);
    CHECK(pf.consumeNextPacketSize() == sizeof(EngineTelemetryPacket) + 4);
    CHECK(pf.consumeNextPacketSize() == sizeof(EngineTelemetryPacket) + 8);
    CHECK(pf.consumeNextPacketSize() == sizeof(EngineTelemetryPacket) + 100);
}

TEST_CASE("Clear packet framer") {
    CircularBuffer buf;
    PacketFramer pf(buf);

    CHECK_FALSE(pf.packetAvailable());
    CHECK(pf.consumeNextPacketSize() == 0);
    for (int i = 0; i < 5; i++) {
        writeTelemetryPacket(buf, pf);
    }
    CHECK(pf.packetAvailable());
    CHECK(pf.consumeNextPacketSize() > 0);
    pf.clear();
    CHECK_FALSE(pf.packetAvailable());
    CHECK(pf.consumeNextPacketSize() == 0);
}
