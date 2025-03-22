#include "MsgBuffer.h"

#include <doctest.h>

TEST_CASE("Can write to and read from MsgBuffer") {
    MsgBuffer msgBuf;
    char* writeData = (char*) ("ACC 1234123459875");
    size_t writeDataSize = 18;

    msgBuf.write(writeData, writeDataSize);
    REQUIRE(msgBuf.availablePackets() == 1);
}