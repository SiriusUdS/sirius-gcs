#include "CircularBuffer.hpp"

#include <doctest.h>

static const size_t TEST_CIRCULAR_BUFFER_SIZE = 10;

bool write(CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE>& buf, const char* str, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!buf.writeChar((uint8_t) str[i])) {
            return false;
        }
    }
    return true;
}

bool fill(CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE>& buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (!buf.writeChar((uint8_t) '0')) {
            return false;
        }
    }
    return true;
}

bool strEqual(uint8_t* str1, const char* str2, size_t size2) {
    for (size_t i = 0; i < size2; i++) {
        if (str1[i] != (uint8_t) str2[i]) {
            return false;
        }
    }
    return true;
}

TEST_CASE("Write to circular buffer") {
    CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
    CHECK(write(buf, "test", 4));
    CHECK(write(buf, "hello!", 6));
}

TEST_CASE("Read from circular buffer") {
    CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
    uint8_t rcv[TEST_CIRCULAR_BUFFER_SIZE];
    CHECK(write(buf, "testing", 7));
    CHECK(buf.read(rcv, 4));
    CHECK(strEqual(rcv, "test", 4));
    CHECK(write(buf, " hello!", 7));
    CHECK(buf.read(rcv, 10));
    CHECK(strEqual(rcv, "ing hello!", 10));
}

TEST_CASE("Clear circular buffer") {
    CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
    CHECK(fill(buf, 10));
    CHECK(buf.isFull());
    buf.clear();
    CHECK_FALSE(buf.isFull());
}

TEST_CASE("Dump data from circular buffer") {
    CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
    CHECK(fill(buf, 10));
    CHECK(buf.isFull());
    CHECK(buf.dump(3));
    CHECK(buf.readAvailable() == 7);
    CHECK(buf.dump(7));
    CHECK(buf.readAvailable() == 0);
}

TEST_CASE("Check available read and write space in circular buffer") {
    CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
    CHECK(buf.readAvailable() == 0);
    CHECK(buf.writeAvailable() == 10);
    CHECK(fill(buf, 1));
    CHECK(buf.readAvailable() == 1);
    CHECK(buf.writeAvailable() == 9);
    CHECK(fill(buf, 9));
    CHECK(buf.readAvailable() == 10);
    CHECK(buf.writeAvailable() == 0);
    CHECK(buf.dump(5));
    CHECK(buf.readAvailable() == 5);
    CHECK(buf.writeAvailable() == 5);
    CHECK(fill(buf, 3));
    CHECK(buf.readAvailable() == 8);
    CHECK(buf.writeAvailable() == 2);
    buf.clear();
    CHECK(buf.readAvailable() == 0);
    CHECK(buf.writeAvailable() == 10);
}

TEST_CASE("Peek back in circular buffer") {
    CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
    CHECK(write(buf, "abcdefg", 7));
    CHECK(buf.peekBack().value() == 'g');
    CHECK(buf.peekBack(1).value() == 'f');
    CHECK(buf.peekBack(3).value() == 'd');
    CHECK(buf.peekBack(6).value() == 'a');
    CHECK(write(buf, "hij", 3));
    CHECK(buf.peekBack().value() == 'j');
    CHECK(buf.peekBack(1).value() == 'i');
    CHECK(buf.peekBack(9).value() == 'a');
    CHECK(buf.dump(3));
    CHECK_FALSE(buf.peekBack(9).has_value());
    CHECK_FALSE(buf.peekBack(7).has_value());
    CHECK_FALSE(buf.peekBack(1000).has_value());
    CHECK(buf.peekBack(6).value() == 'd');
    CHECK(buf.peekBack().value() == 'j');
}
