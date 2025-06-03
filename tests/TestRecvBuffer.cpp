// #include "RecvBuffer.hpp"
// #include "Telecommunication/TelemetryPacket.h"
//
// #include <doctest.h>
// #include <string>
//
// static const size_t TEST_RECVBUFFER_SIZE = 1000;
//
// bool writeTelemetryPacketToRecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf) {
//     EngineTelemetryPacket packet;
//     packet.fields.header.bits.type = TELEMETRY_TYPE_CODE;
//     packet.fields.header.bits.boardId = 1;
//
//     for (uint16_t i = 0; i < ENGINE_ADC_CHANNEL_AMOUNT; i++) {
//         packet.fields.adcValues[i] = i;
//     }
//
//     for (size_t i = 0; i < sizeof(EngineTelemetryPacketFields); i++) {
//         if (!recvBuf.writeChar(packet.data[i])) {
//             return false;
//         }
//     }
//     return true;
// }
//
// bool writeStatusPacketToRecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf) {
//     EngineStatusPacket packet;
//     packet.fields.header.bits.type = STATUS_TYPE_CODE;
//     packet.fields.header.bits.boardId = 1;
//     packet.fields.valveStatus->value = 1010;
//
//     for (size_t i = 0; i < sizeof(EngineStatusPacketFields); i++) {
//         if (!recvBuf.writeChar(packet.data[i])) {
//             return false;
//         }
//     }
//     return true;
// }
//
// bool writeToRecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf, const char* str, size_t size) {
//     for (size_t i = 0; i < size; i++) {
//         if (!recvBuf.writeChar(str[i])) {
//             return false;
//         }
//     }
//     return true;
// }
//
// bool fillRecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf, size_t size) {
//     for (size_t i = 0; i < size; i++) {
//         if (!recvBuf.writeChar(' ')) {
//             return false;
//         }
//     }
//     return true;
// }
//
// TEST_CASE("Should detect correct amount of packets during writes") {
//     RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;
//
//     SUBCASE("Correct header codes") {
//         CHECK(writeTelemetryPacketToRecvBuffer(recvBuf));
//         CHECK(recvBuf.availablePackets() == 0);
//
//         CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//         CHECK(recvBuf.availablePackets() == 1);
//
//         CHECK(writeTelemetryPacketToRecvBuffer(recvBuf));
//         CHECK(recvBuf.availablePackets() == 2);
//
//         for (size_t i = 0; i < 4; i++) {
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//         }
//         CHECK(recvBuf.availablePackets() == 6);
//     }
//
//     SUBCASE("Wrong header codes") {
//         CHECK(writeToRecvBuffer(recvBuf, "ACA -----------", 15));
//         CHECK(recvBuf.availablePackets() == 0);
//
//         CHECK(writeToRecvBuffer(recvBuf, "MXGXR\0SRXT0", 11));
//         CHECK(recvBuf.availablePackets() == 0);
//
//         CHECK(writeToRecvBuffer(recvBuf, "asdfasdfasdfasdfasdfasdf123412341234123412341234", 48));
//         CHECK(recvBuf.availablePackets() == 0);
//
//         CHECK(writeToRecvBuffer(recvBuf, "\0AL\0T\0", 6));
//         CHECK(recvBuf.availablePackets() == 0);
//     }
//
//     SUBCASE("Header codes split between beginning and end of circular buffer") {
//         uint8_t rcv[TEST_RECVBUFFER_SIZE] = {0};
//         size_t dataSize;
//
//         SUBCASE("Split after 1st header code character") {
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(fillRecvBuffer(recvBuf, 911));
//             dataSize = recvBuf.readPacket(rcv);
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(recvBuf.availablePackets() == 1);
//         }
//
//         SUBCASE("Split after 2st header code character") {
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(fillRecvBuffer(recvBuf, 910));
//             dataSize = recvBuf.readPacket(rcv);
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(recvBuf.availablePackets() == 1);
//         }
//
//         SUBCASE("Split after 3st header code character") {
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(fillRecvBuffer(recvBuf, 909));
//             dataSize = recvBuf.readPacket(rcv);
//             CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//             CHECK(recvBuf.availablePackets() == 1);
//         }
//     }
// }
//
// TEST_CASE("Should be able to detect next packet size and dump packets correctly") {
//     RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;
//
//     CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//     CHECK(writeTelemetryPacketToRecvBuffer(recvBuf));
//     CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//     CHECK(writeTelemetryPacketToRecvBuffer(recvBuf));
//     CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//     CHECK(recvBuf.nextPacketSize() == sizeof(EngineStatusPacket));
//
//     CHECK(recvBuf.dumpNextPacket());
//     CHECK(recvBuf.nextPacketSize() == sizeof(EngineTelemetryPacket));
//
//     CHECK(recvBuf.dumpNextPacket());
//     CHECK(recvBuf.nextPacketSize() == sizeof(EngineStatusPacket));
//
//     CHECK(recvBuf.dumpNextPacket());
//     CHECK(recvBuf.nextPacketSize() == sizeof(EngineTelemetryPacket));
//
//     CHECK(recvBuf.dumpNextPacket());
//     CHECK(recvBuf.nextPacketSize() == 0);
// }
//
// TEST_CASE("Should read packets correctly") {
//     RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;
//     uint8_t rcv[TEST_RECVBUFFER_SIZE] = {0};
//     size_t dataSize;
//
//     SUBCASE("Packet with data") {
//         CHECK(writeTelemetryPacketToRecvBuffer(recvBuf));
//         CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//         dataSize = recvBuf.readPacket(rcv);
//         CHECK(dataSize == 44);
//         EngineTelemetryPacket* packet = (EngineTelemetryPacket*) rcv;
//         for (uint16_t i = 0; i < ENGINE_ADC_CHANNEL_AMOUNT; i++) {
//             CHECK(packet->fields.adcValues[i] == i);
//         }
//     }
//
//     SUBCASE("Packet with data split between beginning and end of circular buffer") {
//         CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//         CHECK(fillRecvBuffer(recvBuf, 900));
//         CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//         dataSize = recvBuf.readPacket(rcv);
//         CHECK(dataSize == 900 + sizeof(EngineStatusPacket));
//         CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//         CHECK(writeStatusPacketToRecvBuffer(recvBuf));
//         dataSize = recvBuf.readPacket(rcv);
//         CHECK(dataSize == sizeof(EngineStatusPacket));
//         dataSize = recvBuf.readPacket(rcv);
//         CHECK(dataSize == sizeof(EngineStatusPacket));
//         EngineStatusPacket* packet = (EngineStatusPacket*) rcv;
//         CHECK(packet->fields.valveStatus->value == 1010);
//     }
// }
//
// TEST_CASE("Should automatically clear when full and no packets available") {
//     RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;
//
//     CHECK_FALSE(recvBuf.isFull());
//     CHECK(fillRecvBuffer(recvBuf, TEST_RECVBUFFER_SIZE));
//     CHECK_FALSE(recvBuf.isFull());
//     CHECK(fillRecvBuffer(recvBuf, TEST_RECVBUFFER_SIZE * 2));
//     CHECK_FALSE(recvBuf.isFull());
// }
