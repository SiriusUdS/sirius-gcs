#include "PacketCircularBuffer.h"
#include "PacketFramer.h"
#include "SerialConfig.h"
#include "Telecommunication/PacketHeaderVariable.h"
#include "Telecommunication/TelemetryHeader.h"
#include "Telecommunication/TelemetryPacket.h"

#include <doctest.h>

void writeTelemetryPacket(PacketCircularBuffer& buf, PacketFramer& pf) {
    EngineTelemetryPacket packet;
    packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;
    packet.fields.header.bits.boardId = ENGINE_BOARD_ID;

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

TEST_CASE("PacketFramer should consume next packet metadata") {
    PacketCircularBuffer buf;
    PacketFramer pf(buf);
    PacketMetadata metadata;

    CHECK_FALSE(pf.consumeNextPacketMetadata().has_value());
    writeTelemetryPacket(buf, pf);
    CHECK_FALSE(pf.consumeNextPacketMetadata().has_value());
    writeTelemetryPacket(buf, pf);
    fillCircularBuffer(buf, pf, 4);

    metadata = pf.consumeNextPacketMetadata().value();
    CHECK(metadata.size == sizeof(EngineTelemetryPacket));
    writeTelemetryPacket(buf, pf);
    fillCircularBuffer(buf, pf, 8);
    writeTelemetryPacket(buf, pf);
    fillCircularBuffer(buf, pf, SerialConfig::MAX_PACKET_SIZE + 1 - sizeof(EngineTelemetryPacket));
    writeTelemetryPacket(buf, pf);

    metadata = pf.consumeNextPacketMetadata().value();
    CHECK(metadata.size == sizeof(EngineTelemetryPacket) + 4);
    CHECK(metadata.isValid);
    CHECK(metadata.packetTypeCode == TELEMETRY_TYPE_CODE);
    CHECK(metadata.boardId == ENGINE_BOARD_ID);

    metadata = pf.consumeNextPacketMetadata().value();
    CHECK(metadata.size == sizeof(EngineTelemetryPacket) + 8);
    CHECK(metadata.isValid);
    CHECK(metadata.packetTypeCode == TELEMETRY_TYPE_CODE);
    CHECK(metadata.boardId == ENGINE_BOARD_ID);

    metadata = pf.consumeNextPacketMetadata().value();
    CHECK(metadata.size == SerialConfig::MAX_PACKET_SIZE + 1);
    CHECK_FALSE(metadata.isValid);
    CHECK(metadata.packetTypeCode == TELEMETRY_TYPE_CODE);
    CHECK(metadata.boardId == ENGINE_BOARD_ID);
}

TEST_CASE("PacketFramer should clear correctly") {
    PacketCircularBuffer buf;
    PacketFramer pf(buf);
    PacketMetadata metadata;

    CHECK_FALSE(pf.packetAvailable());
    CHECK_FALSE(pf.consumeNextPacketMetadata().has_value());
    for (int i = 0; i < 5; i++) {
        writeTelemetryPacket(buf, pf);
    }
    CHECK(pf.packetAvailable());
    metadata = pf.consumeNextPacketMetadata().value();
    CHECK(metadata.size > 0);
    pf.clear();
    CHECK_FALSE(pf.packetAvailable());
    CHECK_FALSE(pf.consumeNextPacketMetadata().has_value());
}
