// #include "CircularBuffer.hpp"
//
// #include <doctest.h>
//
// static const size_t TEST_CIRCULAR_BUFFER_SIZE = 10;
//
// bool write(CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf, const char* str, size_t size) {
//     for (size_t i = 0; i < size; i++) {
//         if (!buf.writeChar((uint8_t) str[i])) {
//             return false;
//         }
//     }
//     return true;
// }
//
// bool strEqual(uint8_t* str1, const char* str2, size_t size2) {
//     for (size_t i = 0; i < size2; i++) {
//         if (str1[i] != (uint8_t) str2[i]) {
//             return false;
//         }
//     }
//     return true;
// }
//
// TEST_CASE("Write to circular buffer") {
//     CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
//     CHECK(write(buf, "test", 4));
//     CHECK(write(buf, "hello!", 6));
// }
//
// TEST_CASE("Read from buffer") {
//     CircularBuffer<TEST_CIRCULAR_BUFFER_SIZE> buf;
//     uint8_t rcv[TEST_CIRCULAR_BUFFER_SIZE];
//     CHECK(write(buf, "testing", 7));
//     CHECK(buf.read(rcv, 4));
//     CHECK(strEqual(rcv, "test", 4));
//     CHECK(buf.read(rcv, 3));
//     CHECK(strEqual(rcv, "ing", 3));
// }
