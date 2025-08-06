#include "PacketReceiver.h"
#include "SerialConfig.h"
#include "Telecommunication/PacketHeaderVariable.h"
#include "Telecommunication/TelemetryPacket.h"

#include <doctest.h>

void writeEngineTelemetryPacket(PacketReceiver& pr) {
    EngineTelemetryPacket packet;
    packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;
    packet.fields.header.bits.boardId = ENGINE_BOARD_ID;
    for (uint16_t i = 0; i < ENGINE_ADC_CHANNEL_AMOUNT; i++) {
        packet.fields.adcValues[i] = 0x1234 + i;
    }
    packet.fields.crc = 0x12345678;

    for (size_t i = 0; i < sizeof(EngineTelemetryPacket); i++) {
        pr.receiveByte(packet.data[i]);
    }
}

void writeFillingStationTelemetryPacket(PacketReceiver& pr) {
    FillingStationTelemetryPacket packet;
    packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;
    packet.fields.header.bits.boardId = FILLING_STATION_BOARD_ID;
    for (uint16_t i = 0; i < ENGINE_ADC_CHANNEL_AMOUNT; i++) {
        packet.fields.adcValues[i] = 0x5678 + i;
    }
    packet.fields.crc = 0x87654321;

    for (size_t i = 0; i < sizeof(FillingStationTelemetryPacket); i++) {
        pr.receiveByte(packet.data[i]);
    }
}

void checkEngineTelemetryPacket(PacketReceiver& pr) {
    EngineTelemetryPacket packet;

    CHECK(pr.nextPacketMetadata()->status == PacketMetadata::Status::VALID);
    CHECK(pr.nextPacketMetadata()->size == sizeof(EngineTelemetryPacket));

    pr.getPacket(packet.data);

    CHECK(packet.fields.header.bits.type == TELEMETRY_TYPE_CODE);
    CHECK(packet.fields.header.bits.boardId == ENGINE_BOARD_ID);
    for (uint16_t i = 0; i < ENGINE_ADC_CHANNEL_AMOUNT; i++) {
        CHECK(packet.fields.adcValues[i] == 0x1234 + i);
    }
    CHECK(packet.fields.crc == 0x12345678);
}

void checkFillingStationTelemetryPacket(PacketReceiver& pr) {
    FillingStationTelemetryPacket packet;

    CHECK(pr.nextPacketMetadata()->status == PacketMetadata::Status::VALID);
    CHECK(pr.nextPacketMetadata()->size == sizeof(FillingStationTelemetryPacket));

    pr.getPacket(packet.data);

    CHECK(packet.fields.header.bits.type == TELEMETRY_TYPE_CODE);
    CHECK(packet.fields.header.bits.boardId == FILLING_STATION_BOARD_ID);
    for (uint16_t i = 0; i < ENGINE_ADC_CHANNEL_AMOUNT; i++) {
        CHECK(packet.fields.adcValues[i] == 0x5678 + i);
    }
    CHECK(packet.fields.crc == 0x87654321);
}

void fill(PacketReceiver& pr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        pr.receiveByte(0);
    }
}

TEST_CASE("PacketReceiver should receive packet") {
    PacketReceiver pr;

    CHECK_FALSE(pr.nextPacketMetadata().has_value());
    writeEngineTelemetryPacket(pr);
    CHECK_FALSE(pr.nextPacketMetadata().has_value());
    writeEngineTelemetryPacket(pr);
    CHECK(pr.nextPacketMetadata().has_value());
}

TEST_CASE("PacketReceiver should be able to get packet") {
    PacketReceiver pr;

    writeEngineTelemetryPacket(pr);
    writeEngineTelemetryPacket(pr);
    checkEngineTelemetryPacket(pr);
}

TEST_CASE("PacketReceiver should dump next packet") {
    PacketReceiver pr;
    uint8_t buf[sizeof(EngineTelemetryPacket) + sizeof(FillingStationTelemetryPacket)] = {0};

    writeEngineTelemetryPacket(pr);
    writeEngineTelemetryPacket(pr);
    fill(pr, 4);
    writeFillingStationTelemetryPacket(pr);
    fill(pr, 8);
    CHECK(pr.nextPacketMetadata()->status == PacketMetadata::Status::VALID);
    CHECK(pr.nextPacketMetadata()->size == sizeof(EngineTelemetryPacket));
    CHECK(pr.dumpNextPacket());
    CHECK(pr.nextPacketMetadata()->status == PacketMetadata::Status::VALID);
    CHECK(pr.nextPacketMetadata()->size == sizeof(EngineTelemetryPacket) + 4);
    writeEngineTelemetryPacket(pr);
    fill(pr, 1000);
    CHECK(pr.dumpNextPacket());
    CHECK(pr.nextPacketMetadata()->status == PacketMetadata::Status::VALID);
    CHECK(pr.nextPacketMetadata()->size == sizeof(FillingStationTelemetryPacket) + 8);
    CHECK(pr.dumpNextPacket());
    CHECK_FALSE(pr.nextPacketMetadata().has_value());
    CHECK_FALSE(pr.dumpNextPacket());
}

TEST_CASE("PacketReceiver internal buffer reset") {
    PacketReceiver pr;

    SUBCASE("PacketReceiver should reset when the internal buffer is full and no packets are available") {
        fill(pr, SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE);
        writeFillingStationTelemetryPacket(pr);
        writeEngineTelemetryPacket(pr);
        checkFillingStationTelemetryPacket(pr);
    }

    SUBCASE("PacketReceiver should not reset when the interval buffer is full and packets are available") {
        uint8_t recv[sizeof(EngineTelemetryPacket)] = {0};

        writeEngineTelemetryPacket(pr);
        writeEngineTelemetryPacket(pr);
        CHECK(pr.nextPacketMetadata()->status == PacketMetadata::Status::VALID);
        CHECK(pr.nextPacketMetadata()->size == sizeof(EngineTelemetryPacket));
        fill(pr, SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE - sizeof(EngineTelemetryPacket));
        writeFillingStationTelemetryPacket(pr);
        writeEngineTelemetryPacket(pr);
        writeFillingStationTelemetryPacket(pr);
        checkEngineTelemetryPacket(pr);
        CHECK_FALSE(pr.nextPacketMetadata().has_value());
    }
}

TEST_CASE("PacketReceiver should correctly receive packets even if initial data was junk") {
    PacketReceiver pr;

    fill(pr, 13);
    writeFillingStationTelemetryPacket(pr);
    writeEngineTelemetryPacket(pr);
    checkFillingStationTelemetryPacket(pr);
    writeFillingStationTelemetryPacket(pr);
    checkEngineTelemetryPacket(pr);
}
