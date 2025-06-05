#include "PacketReceiver.h"
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

void fill(PacketReceiver& pr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        pr.receiveByte(0);
    }
}

TEST_CASE("Receive packet with packet receiver") {
    PacketReceiver pr;

    CHECK(pr.nextPacketSize() == 0);
    writeTelemetryPacket(pr);
    CHECK(pr.nextPacketSize() == 0);
    writeTelemetryPacket(pr);
    CHECK(pr.nextPacketSize() == sizeof(EngineTelemetryPacket));
}

TEST_CASE("Get packet from packet receiver") {
    PacketReceiver pr;
    EngineTelemetryPacket packet;

    writeTelemetryPacket(pr);
    writeTelemetryPacket(pr);
    CHECK(pr.getPacket(packet.data));
    CHECK(packet.fields.adcValues[0] == 1234);
}

TEST_CASE("Dump next packet in packet receiver") {
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
