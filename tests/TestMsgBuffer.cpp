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

TEST_CASE("Writes should only be performed if there's enough space left in the buffer") {
    MsgBuffer<TEST_MSGBUFFER_SIZE> msgBuf;

    SUBCASE("Successful write") {
        CHECK(fillMsgBuffer(msgBuf, TEST_MSGBUFFER_SIZE));
    }

    SUBCASE("Failed write") {
        CHECK_FALSE(fillMsgBuffer(msgBuf, TEST_MSGBUFFER_SIZE + 1));
    }
}

TEST_CASE("Should detect correct amount of packets during writes") {
    MsgBuffer<TEST_MSGBUFFER_SIZE> msgBuf;

    SUBCASE("Correct header codes") {
        CHECK(writeToMsgBuffer(msgBuf, "ACC\0a1234acc test", 17));
        CHECK(msgBuf.availablePackets() == 0);

        CHECK(writeToMsgBuffer(msgBuf, "asdfGYR\0qwerty", 14));
        CHECK(msgBuf.availablePackets() == 1);

        CHECK(writeToMsgBuffer(msgBuf, "zxcvbnmVLV\0", 11));
        CHECK(msgBuf.availablePackets() == 2);

        CHECK(writeToMsgBuffer(msgBuf, "ALT\0GPS\0MAG\0abcdefgPRS\0", 23));
        CHECK(msgBuf.availablePackets() == 6);
    }

    SUBCASE("Wrong header codes") {
        CHECK(writeToMsgBuffer(msgBuf, "ACC -----------", 15));
        CHECK(msgBuf.availablePackets() == 0);

        CHECK(writeToMsgBuffer(msgBuf, "MAGPR\0SRKT0", 11));
        CHECK(msgBuf.availablePackets() == 0);

        CHECK(writeToMsgBuffer(msgBuf, "asdfasdfasdfasdfasdfasdf123412341234123412341234", 48));
        CHECK(msgBuf.availablePackets() == 0);

        CHECK(writeToMsgBuffer(msgBuf, "\0AL\0T\0", 6));
        CHECK(msgBuf.availablePackets() == 0);
    }

    SUBCASE("Header codes split through multiple writes") {
        CHECK(writeToMsgBuffer(msgBuf, "1234 A", 6));
        CHECK(writeToMsgBuffer(msgBuf, "CC\0abcd", 7));
        CHECK(msgBuf.availablePackets() == 0);

        CHECK(writeToMsgBuffer(msgBuf, "5678 TH", 7));
        CHECK(writeToMsgBuffer(msgBuf, "M\0efgh", 6));
        CHECK(msgBuf.availablePackets() == 1);

        CHECK(writeToMsgBuffer(msgBuf, "9012 VLV", 8));
        CHECK(writeToMsgBuffer(msgBuf, "\0ijkl", 5));
        CHECK(msgBuf.availablePackets() == 2);

        CHECK(writeToMsgBuffer(msgBuf, "PRS\0", 4));
        CHECK(msgBuf.availablePackets() == 3);
    }

    SUBCASE("Header codes split between beginning and end of circular buffer") {
        char rcv[TEST_MSGBUFFER_SIZE] = {0};
        int headerCode;
        size_t dataSize;

        SUBCASE("Split after 1st header code character") {
            CHECK(writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8));
            CHECK(fillMsgBuffer(msgBuf, 91));
            dataSize = msgBuf.readPacket(&headerCode, rcv);
            CHECK(dataSize == 0);
            CHECK(writeToMsgBuffer(msgBuf, "ACC\0", 4));
            CHECK(msgBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 2st header code character") {
            CHECK(writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8));
            CHECK(fillMsgBuffer(msgBuf, 90));
            dataSize = msgBuf.readPacket(&headerCode, rcv);
            CHECK(dataSize == 0);
            CHECK(writeToMsgBuffer(msgBuf, "ACC\0", 4));
            CHECK(msgBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 3st header code character") {
            CHECK(writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8));
            CHECK(fillMsgBuffer(msgBuf, 89));
            dataSize = msgBuf.readPacket(&headerCode, rcv);
            CHECK(dataSize == 0);
            CHECK(writeToMsgBuffer(msgBuf, "ACC\0", 4));
            CHECK(msgBuf.availablePackets() == 1);
        }
    }
}

TEST_CASE("Should read packets correctly") {
    MsgBuffer<TEST_MSGBUFFER_SIZE> msgBuf;
    int headerCode;
    char rcv[TEST_MSGBUFFER_SIZE] = {0};
    size_t dataSize;

    SUBCASE("Packet with no data") {
        CHECK(writeToMsgBuffer(msgBuf, "ACC\0ACC\0", 8));
        dataSize = msgBuf.readPacket(&headerCode, rcv);
        CHECK(dataSize == 0);
        CHECK(headerCode == ACCELEROMETER_DATA_HEADER_CODE);
    }

    SUBCASE("Packet with data") {
        CHECK(writeToMsgBuffer(msgBuf, "PRS\0abcd1234RKT\0", 16));
        dataSize = msgBuf.readPacket(&headerCode, rcv);
        CHECK(dataSize == 8);
        CHECK(headerCode == PRESSURE_SENSOR_DATA_HEADER_CODE);
        CHECK(msgEqual(rcv, "abcd1234", dataSize));
    }

    SUBCASE("Packet with data split between beginning and end of circular buffer") {
        CHECK(writeToMsgBuffer(msgBuf, "ACC\0", 4));
        CHECK(fillMsgBuffer(msgBuf, 84));
        CHECK(writeToMsgBuffer(msgBuf, "ACC\0", 4));
        dataSize = msgBuf.readPacket(&headerCode, rcv); // Clear packet (0-87)
        CHECK(dataSize == 84);
        CHECK(writeToMsgBuffer(msgBuf, "ALT\0efgh5678GPS\0", 16));
        dataSize = msgBuf.readPacket(&headerCode, rcv); // Clear packet (88-91)
        CHECK(dataSize == 0);
        dataSize = msgBuf.readPacket(&headerCode, rcv); // Read target packet (92-7)
        CHECK(dataSize == 8);
        CHECK(headerCode == ALTIMETER_DATA_HEADER_CODE);
        CHECK(msgEqual(rcv, "efgh5678", dataSize));
    }
}
