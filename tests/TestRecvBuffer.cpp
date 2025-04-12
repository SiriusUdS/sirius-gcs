#include "RecvBuffer.hpp"

#include <doctest.h>
#include <string>

static const size_t TEST_RECVBUFFER_SIZE = 100;

bool writeTorecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf, const char* str, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!recvBuf.writeChar(str[i])) {
            return false;
        }
    }
    return true;
}

bool fillrecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!recvBuf.writeChar(' ')) {
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
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;

    SUBCASE("Successful write") {
        CHECK(fillrecvBuffer(recvBuf, TEST_RECVBUFFER_SIZE));
    }

    SUBCASE("Failed write") {
        CHECK_FALSE(fillrecvBuffer(recvBuf, TEST_RECVBUFFER_SIZE + 1));
    }
}

TEST_CASE("Should detect correct amount of packets during writes") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;

    SUBCASE("Correct header codes") {
        CHECK(writeTorecvBuffer(recvBuf, "ACC\0a1234acc test", 17));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeTorecvBuffer(recvBuf, "asdfGYR\x002qwerty", 14));
        CHECK(recvBuf.availablePackets() == 1);

        CHECK(writeTorecvBuffer(recvBuf, "zxcvbnmVLV\x010", 11));
        CHECK(recvBuf.availablePackets() == 2);

        CHECK(writeTorecvBuffer(recvBuf, "ALT\0GPS\x044MAG\0abcdefgPRS\0", 23));
        CHECK(recvBuf.availablePackets() == 6);
    }

    SUBCASE("Wrong header codes") {
        CHECK(writeTorecvBuffer(recvBuf, "ACA -----------", 15));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeTorecvBuffer(recvBuf, "MXGXR\0SRXT0", 11));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeTorecvBuffer(recvBuf, "asdfasdfasdfasdfasdfasdf123412341234123412341234", 48));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeTorecvBuffer(recvBuf, "\0AL\0T\0", 6));
        CHECK(recvBuf.availablePackets() == 0);
    }

    SUBCASE("Header codes split through multiple writes") {
        CHECK(writeTorecvBuffer(recvBuf, "1234 A", 6));
        CHECK(writeTorecvBuffer(recvBuf, "CC\0abcd", 7));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeTorecvBuffer(recvBuf, "5678 TH", 7));
        CHECK(writeTorecvBuffer(recvBuf, "M\0efgh", 6));
        CHECK(recvBuf.availablePackets() == 1);

        CHECK(writeTorecvBuffer(recvBuf, "9012 VLV", 8));
        CHECK(writeTorecvBuffer(recvBuf, "\x001ijkl", 5));
        CHECK(recvBuf.availablePackets() == 2);

        CHECK(writeTorecvBuffer(recvBuf, "PRS\0", 4));
        CHECK(recvBuf.availablePackets() == 3);
    }

    SUBCASE("Header codes split between beginning and end of circular buffer") {
        char rcv[TEST_RECVBUFFER_SIZE] = {0};
        size_t dataSize;

        SUBCASE("Split after 1st header code character") {
            CHECK(writeTorecvBuffer(recvBuf, "ACC\0ACC\0", 8));
            CHECK(fillrecvBuffer(recvBuf, 91));
            dataSize = recvBuf.readPacket(rcv);
            CHECK(dataSize == 4);
            CHECK(writeTorecvBuffer(recvBuf, "ACC\0", 4));
            CHECK(recvBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 2st header code character") {
            CHECK(writeTorecvBuffer(recvBuf, "ACC\0ACC\0", 8));
            CHECK(fillrecvBuffer(recvBuf, 90));
            dataSize = recvBuf.readPacket(rcv);
            CHECK(dataSize == 4);
            CHECK(writeTorecvBuffer(recvBuf, "ACC\0", 4));
            CHECK(recvBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 3st header code character") {
            CHECK(writeTorecvBuffer(recvBuf, "ACC\0ACC\0", 8));
            CHECK(fillrecvBuffer(recvBuf, 89));
            dataSize = recvBuf.readPacket(rcv);
            CHECK(dataSize == 4);
            CHECK(writeTorecvBuffer(recvBuf, "ACC\0", 4));
            CHECK(recvBuf.availablePackets() == 1);
        }
    }
}

TEST_CASE("Should read packets correctly") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;
    char rcv[TEST_RECVBUFFER_SIZE] = {0};
    size_t dataSize;

    SUBCASE("Packet with no data") {
        CHECK(writeTorecvBuffer(recvBuf, "ACC\0ACC\x001", 8));
        dataSize = recvBuf.readPacket(rcv);
        CHECK(dataSize == 4);
    }

    SUBCASE("Packet with data") {
        CHECK(writeTorecvBuffer(recvBuf, "PRS\0abcd1234RKT\0", 16));
        dataSize = recvBuf.readPacket(rcv);
        CHECK(dataSize == 12);
        CHECK(msgEqual(rcv, "PRS\0abcd1234", dataSize));
    }

    SUBCASE("Packet with data split between beginning and end of circular buffer") {
        CHECK(writeTorecvBuffer(recvBuf, "ACC\0", 4));
        CHECK(fillrecvBuffer(recvBuf, 84));
        CHECK(writeTorecvBuffer(recvBuf, "ACC\0", 4));
        dataSize = recvBuf.readPacket(rcv); // Clear packet (0-87)
        CHECK(dataSize == 88);
        CHECK(writeTorecvBuffer(recvBuf, "ALT\0efgh5678GPS\0", 16));
        dataSize = recvBuf.readPacket(rcv); // Clear packet (88-91)
        CHECK(dataSize == 4);
        dataSize = recvBuf.readPacket(rcv); // Read target packet (92-7)
        CHECK(dataSize == 12);
        CHECK(msgEqual(rcv, "ALT\0efgh5678", dataSize));
    }
}

TEST_CASE("Should detect when the buffer is full") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;
    char rcv[TEST_RECVBUFFER_SIZE];
    size_t dataSize;

    CHECK_FALSE(recvBuf.isFull());
    CHECK(writeTorecvBuffer(recvBuf, "ACC\0", 4));
    CHECK(fillrecvBuffer(recvBuf, 46));
    CHECK_FALSE(recvBuf.isFull());
    CHECK(writeTorecvBuffer(recvBuf, "ACC\0", 4));
    CHECK(fillrecvBuffer(recvBuf, 46));
    CHECK(recvBuf.isFull());
    dataSize = recvBuf.readPacket(rcv);
    CHECK(dataSize == 50);
    CHECK_FALSE(recvBuf.isFull());
    CHECK(fillrecvBuffer(recvBuf, 50));
    CHECK(recvBuf.isFull());
}

TEST_CASE("Should clear correctly") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;

    CHECK(writeTorecvBuffer(recvBuf, "ACC\0abcdVLV\x00AzaaaaaaaGPS\0qwertyazertyTHM\0", 40));
    recvBuf.clear();
    CHECK(recvBuf.availablePackets() == 0);
    CHECK_FALSE(recvBuf.isFull());
}
