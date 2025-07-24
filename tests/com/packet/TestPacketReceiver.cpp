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

void fill(PacketReceiver& pr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        pr.receiveByte(0);
    }
}

TEST_CASE("PacketReceiver should receive packet") {
    PacketReceiver pr;

    CHECK(pr.nextPacketSize() == 0);
    writeTelemetryPacket(pr);
    CHECK(pr.nextPacketSize() == 0);
    writeTelemetryPacket(pr);
    CHECK(pr.nextPacketSize() == sizeof(EngineTelemetryPacket));
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
    CHECK(pr.nextPacketSize() == sizeof(EngineTelemetryPacket));
    CHECK(pr.dumpNextPacket());
    CHECK(pr.nextPacketSize() == sizeof(EngineTelemetryPacket) + 4);
    writeTelemetryPacket(pr);
    fill(pr, 1000);
    CHECK(pr.dumpNextPacket());
    CHECK(pr.nextPacketSize() == sizeof(EngineTelemetryPacket) + 8);
    CHECK(pr.dumpNextPacket());
    CHECK(pr.nextPacketSize() == 0);
    CHECK_FALSE(pr.dumpNextPacket());
}

TEST_CASE("PacketReceiver internal buffer reset") {
    PacketReceiver pr;

    SUBCASE("PacketReceiver should reset when the internal buffer is full and no packets are available") {
        fill(pr, SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE);
        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        CHECK(pr.nextPacketSize() == sizeof(EngineTelemetryPacket));
    }

    SUBCASE("PacketReceiver should not reset when the interval buffer is full and packet are available") {
        uint8_t recv[sizeof(EngineTelemetryPacket)] = {0};

        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        fill(pr, SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE - sizeof(EngineTelemetryPacket));
        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        writeTelemetryPacket(pr);
        CHECK(pr.nextPacketSize() == sizeof(EngineTelemetryPacket));
        pr.getPacket(recv);
        CHECK(pr.nextPacketSize() == 0);
    }
}
