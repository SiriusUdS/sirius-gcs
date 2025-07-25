#include "PacketCircularBuffer.h"
#include "PacketFramer.h"
#include "Telecommunication/PacketHeaderVariable.h"
#include "Telecommunication/TelemetryHeader.h"
#include "Telecommunication/TelemetryPacket.h"

#include <doctest.h>

constexpr size_t ENGINE_TELEMETRY_PACKET_BODY_SIZE = sizeof(EngineTelemetryPacket) - sizeof(TelemetryHeader);

void writeTelemetryPacketHeader(PacketCircularBuffer& buf, PacketFramer& pf) {
    TelemetryHeader header;
    header.bits.type = TELEMETRY_TYPE_CODE;
    header.bits.boardId = ENGINE_BOARD_ID;
    for (uint16_t i = 0; i < sizeof(TelemetryHeader); i++) {
        uint8_t byte = ((uint8_t*) &header)[i];
        buf.writeByte(byte);
        pf.byteWritten();
    }
}

void writeTelemetryPacketBody(PacketCircularBuffer& buf, PacketFramer& pf) {
    EngineTelemetryPacket packet;
    for (uint16_t i = 0; i < ENGINE_ADC_CHANNEL_AMOUNT; i++) {
        packet.fields.adcValues[i] = 0x1234 + i;
    }
    packet.fields.crc = 0x12345678;

    for (size_t i = sizeof(TelemetryHeader); i < sizeof(EngineTelemetryPacket); i++) {
        uint8_t byte = packet.data[i];
        buf.writeByte(byte);
        pf.byteWritten();
    }
}

void fillCircularBuffer(PacketCircularBuffer& buf, PacketFramer& pf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf.writeByte(0);
        pf.byteWritten();
    }
}

TEST_CASE("PacketFramer should detect packets") {
    PacketCircularBuffer buf;
    PacketFramer pf(buf);

    CHECK_FALSE(pf.tryFrame());
    CHECK_FALSE(pf.getLastPacketMetadata());

    writeTelemetryPacketBody(buf, pf);
    CHECK_FALSE(pf.tryFrame());
    CHECK_FALSE(pf.getLastPacketMetadata());

    writeTelemetryPacketHeader(buf, pf);
    CHECK(pf.tryFrame());
    CHECK(pf.getLastPacketMetadata());
    CHECK(pf.getLastPacketMetadata()->status == PacketMetadata::Status::DUMP_IMMEDIATLY);
    CHECK(pf.getLastPacketMetadata()->size == ENGINE_TELEMETRY_PACKET_BODY_SIZE);

    pf.newPacket();

    writeTelemetryPacketBody(buf, pf);
    CHECK_FALSE(pf.tryFrame());
    CHECK(pf.getLastPacketMetadata());
    CHECK(pf.getLastPacketMetadata()->status == PacketMetadata::Status::DUMP_IMMEDIATLY);
    CHECK(pf.getLastPacketMetadata()->size == ENGINE_TELEMETRY_PACKET_BODY_SIZE);

    writeTelemetryPacketBody(buf, pf);
    writeTelemetryPacketHeader(buf, pf);
    CHECK(pf.tryFrame());
    CHECK(pf.getLastPacketMetadata());
    CHECK(pf.getLastPacketMetadata()->status == PacketMetadata::Status::VALID);
    CHECK(pf.getLastPacketMetadata()->size == sizeof(EngineTelemetryPacket) + ENGINE_TELEMETRY_PACKET_BODY_SIZE);
}

TEST_CASE("PacketFramer should clear correctly") {
    PacketCircularBuffer buf;
    PacketFramer pf(buf);

    CHECK_FALSE(pf.getLastPacketMetadata());

    writeTelemetryPacketHeader(buf, pf);
    CHECK(pf.tryFrame());
    CHECK(pf.getLastPacketMetadata());
    CHECK(pf.getLastPacketMetadata()->status == PacketMetadata::Status::DUMP_IMMEDIATLY);
    CHECK(pf.getLastPacketMetadata()->size == 0);

    pf.clear();
    CHECK_FALSE(pf.getLastPacketMetadata());
    CHECK_FALSE(pf.tryFrame());
    CHECK_FALSE(pf.getLastPacketMetadata());

    writeTelemetryPacketHeader(buf, pf);
    CHECK(pf.tryFrame());
    CHECK(pf.getLastPacketMetadata());
    CHECK(pf.getLastPacketMetadata()->status == PacketMetadata::Status::DUMP_IMMEDIATLY);
    CHECK(pf.getLastPacketMetadata()->size == 0);

    writeTelemetryPacketBody(buf, pf);
    writeTelemetryPacketHeader(buf, pf);
    CHECK(pf.tryFrame());
    CHECK(pf.getLastPacketMetadata());
    CHECK(pf.getLastPacketMetadata()->status == PacketMetadata::Status::VALID);
    CHECK(pf.getLastPacketMetadata()->size == sizeof(EngineTelemetryPacket));

    pf.clear();
    CHECK_FALSE(pf.getLastPacketMetadata());
    CHECK_FALSE(pf.tryFrame());
    CHECK_FALSE(pf.getLastPacketMetadata());
}
