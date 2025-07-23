#include "PacketReceiver.h"
#include "SerialConfig.h"
#include "Telecommunication/TelemetryPacket.h"

#include <doctest.h>

void writeTelemetryPacket(PacketReceiver& pr) {
    EngineTelemetryPacket packet;
    packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;
    packet.fields.adcValues[0] = 1234;

    for (size_t i = 0; i < sizeof(EngineTelemetryPacket); i++) {
        pr.receiveByte(packet.data[i]);
    }
}

void writeIncompleteTelemetryPacket(PacketReceiver& pr) {
    EngineTelemetryPacket packet;
    packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;
    packet.fields.adcValues[0] = 1234;

    for (size_t i = 0; i < sizeof(EngineTelemetryPacket) / 2; i++) {
        pr.receiveByte(packet.data[i]);
    }
}

uint32_t nextPacketSize(PacketReceiver& pr) {
    std::optional optionalMetadata = pr.nextPacketMetadata();
    if (!optionalMetadata.has_value()) {
        return 0;
    }
    return optionalMetadata.value().size;
}

void fill(PacketReceiver& pr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        pr.receiveByte(0);
    }
}

TEST_CASE("PacketReceiver should receive packet") {
    PacketReceiver pr;

    CHECK(nextPacketSize(pr) == 0);
    writeTelemetryPacket(pr);
    CHECK(nextPacketSize(pr) == 0);
    writeTelemetryPacket(pr);
    CHECK(nextPacketSize(pr) == sizeof(EngineTelemetryPacket));
}

TEST_CASE("PacketReceiver should be able to get packet") {
    PacketReceiver pr;
    EngineTelemetryPacket packet;

    writeTelemetryPacket(pr);
    writeTelemetryPacket(pr);
    CHECK(pr.getPacket(packet.data));
    CHECK(packet.fields.adcValues[0] == 1234);
}

TEST_CASE("PacketReceiver should dump next packet") {
    PacketReceiver pr;

    writeTelemetryPacket(pr);
    writeTelemetryPacket(pr);
    fill(pr, 4);
    writeTelemetryPacket(pr);
    fill(pr, 8);
    CHECK(nextPacketSize(pr) == sizeof(EngineTelemetryPacket));
    CHECK(pr.dumpNextPacket());
    CHECK(nextPacketSize(pr) == sizeof(EngineTelemetryPacket) + 4);
    writeTelemetryPacket(pr);
    fill(pr, 1000);
    CHECK(pr.dumpNextPacket());
    CHECK(nextPacketSize(pr) == sizeof(EngineTelemetryPacket) + 8);
    CHECK(pr.dumpNextPacket());
    CHECK(nextPacketSize(pr) == 0);
    CHECK_FALSE(pr.dumpNextPacket());
}

TEST_CASE("PacketReceiver internal buffer reset") {
    PacketReceiver pr;

    SUBCASE("PacketReceiver should reset when the internal buffer is full and no packets are available") {
        fill(pr, SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE);
        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        CHECK(nextPacketSize(pr) == sizeof(EngineTelemetryPacket));
    }

    SUBCASE("PacketReceiver should not reset when the interval buffer is full and packet are available") {
        uint8_t recv[sizeof(EngineTelemetryPacket)] = {0};

        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        fill(pr, SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE - sizeof(EngineTelemetryPacket));
        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        CHECK(nextPacketSize(pr) == sizeof(EngineTelemetryPacket));
        pr.getPacket(recv);
        CHECK(nextPacketSize(pr) == 0);
    }
}
