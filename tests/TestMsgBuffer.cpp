#include "MsgBuffer.h"

#include <doctest.h>
#include <string>

bool writeToMsgBuffer(MsgBuffer& msgBuf, const char* str, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!msgBuf.writeChar(str[i])) {
            return false;
        }
    }
    return true;
}

TEST_CASE("Should detect correct amount of packets during writes") {
    MsgBuffer msgBuf;

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
}