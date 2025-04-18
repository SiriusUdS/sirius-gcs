#include "RecvBuffer.hpp"

#include <doctest.h>
#include <string>

static const size_t TEST_RECVBUFFER_SIZE = 100;

bool writeToRecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf, const char* str, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!recvBuf.writeChar(str[i])) {
            return false;
        }
    }
    return true;
}

bool fillRecvBuffer(RecvBuffer<TEST_RECVBUFFER_SIZE>& recvBuf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!recvBuf.writeChar(' ')) {
            return false;
        }
    }
    return true;
}

bool msgEqual(uint8_t* rcv, const char* expected, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (rcv[i] != expected[i]) {
            return false;
        }
    }
    return true;
}

TEST_CASE("Should detect correct amount of packets during writes") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;

    SUBCASE("Correct header codes") {
        CHECK(writeToRecvBuffer(recvBuf, "\0CCAa1234acc test", 17));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeToRecvBuffer(recvBuf, "asdf\x002RYGqwerty", 14));
        CHECK(recvBuf.availablePackets() == 1);

        CHECK(writeToRecvBuffer(recvBuf, "zxcvbnm\x010VLV", 11));
        CHECK(recvBuf.availablePackets() == 2);

        CHECK(writeToRecvBuffer(recvBuf, "\0TLA\x044SPG\0GAMabcdefg\0SRP", 23));
        CHECK(recvBuf.availablePackets() == 6);
    }

    SUBCASE("Wrong header codes") {
        CHECK(writeToRecvBuffer(recvBuf, "ACA -----------", 15));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeToRecvBuffer(recvBuf, "MXGXR\0SRXT0", 11));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeToRecvBuffer(recvBuf, "asdfasdfasdfasdfasdfasdf123412341234123412341234", 48));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeToRecvBuffer(recvBuf, "\0AL\0T\0", 6));
        CHECK(recvBuf.availablePackets() == 0);
    }

    SUBCASE("Header codes split through multiple writes") {
        CHECK(writeToRecvBuffer(recvBuf, "1234 \0C", 7));
        CHECK(writeToRecvBuffer(recvBuf, "CAabcd", 6));
        CHECK(recvBuf.availablePackets() == 0);

        CHECK(writeToRecvBuffer(recvBuf, "5678 \0M", 7));
        CHECK(writeToRecvBuffer(recvBuf, "HTefgh", 6));
        CHECK(recvBuf.availablePackets() == 1);

        CHECK(writeToRecvBuffer(recvBuf, "9012 \x001", 8));
        CHECK(writeToRecvBuffer(recvBuf, "VLVijkl", 7));
        CHECK(recvBuf.availablePackets() == 2);

        CHECK(writeToRecvBuffer(recvBuf, "\0SRP", 4));
        CHECK(recvBuf.availablePackets() == 3);
    }

    SUBCASE("Header codes split between beginning and end of circular buffer") {
        uint8_t rcv[TEST_RECVBUFFER_SIZE] = {0};
        size_t dataSize;

        SUBCASE("Split after 1st header code character") {
            CHECK(writeToRecvBuffer(recvBuf, "\0CCA\0CCA", 8));
            CHECK(fillRecvBuffer(recvBuf, 91));
            dataSize = recvBuf.readPacket(rcv);
            CHECK(dataSize == 4);
            CHECK(writeToRecvBuffer(recvBuf, "\0CCA", 4));
            CHECK(recvBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 2st header code character") {
            CHECK(writeToRecvBuffer(recvBuf, "\0CCA\0CCA", 8));
            CHECK(fillRecvBuffer(recvBuf, 90));
            dataSize = recvBuf.readPacket(rcv);
            CHECK(dataSize == 4);
            CHECK(writeToRecvBuffer(recvBuf, "\0CCA", 4));
            CHECK(recvBuf.availablePackets() == 1);
        }

        SUBCASE("Split after 3st header code character") {
            CHECK(writeToRecvBuffer(recvBuf, "\0CCA\0CCA", 8));
            CHECK(fillRecvBuffer(recvBuf, 89));
            dataSize = recvBuf.readPacket(rcv);
            CHECK(dataSize == 4);
            CHECK(writeToRecvBuffer(recvBuf, "\0CCA", 4));
            CHECK(recvBuf.availablePackets() == 1);
        }
    }
}

TEST_CASE("Should correctly detect next packet header and size") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;

    CHECK(writeToRecvBuffer(recvBuf, "\0CCAa1234acc test", 17));
    CHECK(recvBuf.nextPacketHeaderCode() == 0);
    CHECK(recvBuf.nextPacketSize() == 0);

    CHECK(writeToRecvBuffer(recvBuf, "asdf\x002RYGqwerty", 14));
    CHECK(recvBuf.nextPacketHeaderCode() == ACCELEROMETER_DATA_HEADER_CODE);
    CHECK(recvBuf.nextPacketSize() == 21);

    CHECK(writeToRecvBuffer(recvBuf, "zxcvbnm\x010VLV", 11));
    CHECK(recvBuf.nextPacketHeaderCode() == ACCELEROMETER_DATA_HEADER_CODE);
    CHECK(recvBuf.nextPacketSize() == 21);

    CHECK(recvBuf.dumpNextPacket());
    CHECK(recvBuf.nextPacketHeaderCode() == GYROSCOPE_DATA_HEADER_CODE);
    CHECK(recvBuf.nextPacketSize() == 17);
}

TEST_CASE("Should be able to dump packets correctly") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;

    CHECK(writeToRecvBuffer(recvBuf, "\0CCA1234\0VLV567876\0TLA90\0SPG3456\0CCA7890", 42));
    CHECK(recvBuf.nextPacketHeaderCode() == ACCELEROMETER_DATA_HEADER_CODE);
    CHECK(recvBuf.nextPacketSize() == 8);

    CHECK(recvBuf.dumpNextPacket());
    CHECK(recvBuf.nextPacketHeaderCode() == VALVE_DATA_HEADER_CODE);
    CHECK(recvBuf.nextPacketSize() == 10);

    CHECK(recvBuf.dumpNextPacket());
    CHECK(recvBuf.nextPacketHeaderCode() == ALTIMETER_DATA_HEADER_CODE);
    CHECK(recvBuf.nextPacketSize() == 6);

    CHECK(recvBuf.dumpNextPacket());
    CHECK(recvBuf.nextPacketHeaderCode() == GPS_DATA_HEADER_CODE);
    CHECK(recvBuf.nextPacketSize() == 8);

    CHECK(recvBuf.dumpNextPacket());
    CHECK(recvBuf.nextPacketHeaderCode() == 0);
    CHECK(recvBuf.nextPacketSize() == 0);
}

TEST_CASE("Should read packets correctly") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;
    uint8_t rcv[TEST_RECVBUFFER_SIZE] = {0};
    size_t dataSize;

    SUBCASE("Packet with no data") {
        CHECK(writeToRecvBuffer(recvBuf, "\0CCA\0CCA", 8));
        dataSize = recvBuf.readPacket(rcv);
        CHECK(dataSize == 4);
    }

    SUBCASE("Packet with data") {
        CHECK(writeToRecvBuffer(recvBuf, "\0SRPabcd1234\0TKR", 16));
        dataSize = recvBuf.readPacket(rcv);
        CHECK(dataSize == 12);
        CHECK(msgEqual(rcv, "\0SRPabcd1234", dataSize));
    }

    SUBCASE("Packet with data split between beginning and end of circular buffer") {
        CHECK(writeToRecvBuffer(recvBuf, "\0CCA", 4));
        CHECK(fillRecvBuffer(recvBuf, 84));
        CHECK(writeToRecvBuffer(recvBuf, "\0CCA", 4));
        dataSize = recvBuf.readPacket(rcv); // Clear packet (0-87)
        CHECK(dataSize == 88);
        CHECK(writeToRecvBuffer(recvBuf, "\0TLAefgh5678\0SPG", 16));
        dataSize = recvBuf.readPacket(rcv); // Clear packet (88-91)
        CHECK(dataSize == 4);
        dataSize = recvBuf.readPacket(rcv); // Read target packet (92-7)
        CHECK(dataSize == 12);
        CHECK(msgEqual(rcv, "\0TLAefgh5678", dataSize));
    }
}

TEST_CASE("Should automatically clear when full and no packets available") {
    RecvBuffer<TEST_RECVBUFFER_SIZE> recvBuf;

    CHECK_FALSE(recvBuf.isFull());
    CHECK(fillRecvBuffer(recvBuf, TEST_RECVBUFFER_SIZE));
    CHECK_FALSE(recvBuf.isFull());
    CHECK(fillRecvBuffer(recvBuf, TEST_RECVBUFFER_SIZE * 2));
    CHECK_FALSE(recvBuf.isFull());
}
