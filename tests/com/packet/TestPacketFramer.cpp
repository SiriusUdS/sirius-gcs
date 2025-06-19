#include "PacketCircularBuffer.h"
#include "PacketFramer.h"
#include "Telecommunication/TelemetryHeader.h"
#include "Telecommunication/TelemetryPacket.h"

#include <doctest.h>

void writeTelemetryPacket(PacketCircularBuffer& buf, PacketFramer& pf) {
    EngineTelemetryPacket packet;
    packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;

    for (size_t i = 0; i < sizeof(EngineTelemetryPacket); i++) {
        uint8_t byte = packet.data[i];
        buf.writeByte(byte);
        pf.byteWritten();
        pf.tryFrame();
    }
}

void fillCircularBuffer(PacketCircularBuffer& buf, PacketFramer& pf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf.writeByte(0);
        pf.byteWritten();
        pf.tryFrame();
    }
}

TEST_CASE("PacketFramer should detect packets") {
    PacketCircularBuffer buf;
    PacketFramer pf(buf);

    CHECK_FALSE(pf.packetAvailable());
    writeTelemetryPacket(buf, pf);
    CHECK_FALSE(pf.packetAvailable());
    writeTelemetryPacket(buf, pf);
    CHECK(pf.packetAvailable());
}

TEST_CASE("PacketFramer should consume next packet size") {
    PacketCircularBuffer buf;
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

TEST_CASE("PacketFramer should clear correctly") {
    PacketCircularBuffer buf;
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
