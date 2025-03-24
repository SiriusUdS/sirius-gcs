#include "MsgBuffer.h"

#include <doctest.h>
#include <string>

static const size_t TEST_MSGBUFFER_SIZE = 100;

bool writeToMsgBuffer(MsgBuffer<TEST_MSGBUFFER_SIZE>& msgBuf, const char* str, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!msgBuf.writeChar(str[i])) {
            return false;
        }
    }
    return true;
}

bool fillMsgBuffer(MsgBuffer<TEST_MSGBUFFER_SIZE>& msgBuf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!msgBuf.writeChar(' ')) {
            return false;
        }
    }
    return true;
}

bool msgEqual(char* rcv, const char* expected, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (rcv[i] != expected[i]) {
            return false;
        }
    }
    return true;
}

TEST_CASE("Should detect correct amount of packets during writes") {
    MsgBuffer<TEST_MSGBUFFER_SIZE> msgBuf;

    SUBCASE("Correct header codes") {
        writeToMsgBuffer(msgBuf, "ACC\0a1234acc test", 17);
        CHECK(msgBuf.availablePackets() == 0);

        writeToMsgBuffer(msgBuf, "asdfGYR\0qwerty", 14);
        CHECK(msgBuf.availablePackets() == 1);

        writeToMsgBuffer(msgBuf, "zxcvbnmVLV\0", 11);
        CHECK(msgBuf.availablePackets() == 2);

        writeToMsgBuffer(msgBuf, "ALT\0GPS\0MAG\0abcdefgPRS\0", 23);
        CHECK(msgBuf.availablePackets() == 6);
    }

    SUBCASE("Wrong header codes") {
        writeToMsgBuffer(msgBuf, "ACC -----------", 15);
        CHECK(msgBuf.availablePackets() == 0);

        writeToMsgBuffer(msgBuf, "MAGPR\0SRKT0", 11);
        CHECK(msgBuf.availablePackets() == 0);

        writeToMsgBuffer(msgBuf, "asdfasdfasdfasdfasdfasdf123412341234123412341234", 48);
        CHECK(msgBuf.availablePackets() == 0);

        writeToMsgBuffer(msgBuf, "\0AL\0T\0", 6);
        CHECK(msgBuf.availablePackets() == 0);
    }

    SUBCASE("Header codes split through multiple writes") {
        writeToMsgBuffer(msgBuf, "1234 A", 6);
        writeToMsgBuffer(msgBuf, "CC\0abcd", 7);
        CHECK(msgBuf.availablePackets() == 0);

        writeToMsgBuffer(msgBuf, "5678 TH", 7);
        writeToMsgBuffer(msgBuf, "M\0efgh", 6);
        CHECK(msgBuf.availablePackets() == 1);

        writeToMsgBuffer(msgBuf, "9012 VLV", 8);
        writeToMsgBuffer(msgBuf, "\0ijkl", 5);
        CHECK(msgBuf.availablePackets() == 2);

        writeToMsgBuffer(msgBuf, "PRS\0", 4);
        CHECK(msgBuf.availablePackets() == 3);
    }

    SUBCASE("Header codes split between beginning and end of circular buffer") {
        char rcv[TEST_MSGBUFFER_SIZE] = {0};
        int headerCode;

        SUBCASE("Split after 1st header code character") {
            writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8);
            fillMsgBuffer(msgBuf, 91);
            msgBuf.readPacket(&headerCode, rcv);
            writeToMsgBuffer(msgBuf, "ACC\0", 4);
            CHECK(msgBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 2st header code character") {
            writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8);
            fillMsgBuffer(msgBuf, 90);
            msgBuf.readPacket(&headerCode, rcv);
            writeToMsgBuffer(msgBuf, "ACC\0", 4);
            CHECK(msgBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 3st header code character") {
            writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8);
            fillMsgBuffer(msgBuf, 89);
            msgBuf.readPacket(&headerCode, rcv);
            writeToMsgBuffer(msgBuf, "ACC\0", 4);
            CHECK(msgBuf.availablePackets() == 1);
        }
    }
}

TEST_CASE("Should read packets correctly") {
    MsgBuffer<TEST_MSGBUFFER_SIZE> msgBuf;
    int headerCode;
    char rcv[TEST_MSGBUFFER_SIZE] = {0};

    SUBCASE("Packet with no data") {
        writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8);
        size_t dataSize = msgBuf.readPacket(&headerCode, rcv);
        CHECK(dataSize == 0);
        CHECK(headerCode == ACCELEROMETER_DATA_HEADER_CODE);
    }

    SUBCASE("Packet with data") {
        writeToMsgBuffer(msgBuf, "PRS\0abcd1234RKT\0", 16);
        size_t dataSize = msgBuf.readPacket(&headerCode, rcv);
        CHECK(dataSize == 8);
        CHECK(headerCode == PRESSURE_SENSOR_DATA_HEADER_CODE);
        CHECK(msgEqual(rcv, "abcd1234", dataSize));
    }
}
