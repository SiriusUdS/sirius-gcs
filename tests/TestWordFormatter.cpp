#include "WordFormatter.h"

#include <doctest.h>

TEST_CASE("16 bit words should be flipped correctly") {
    CHECK_EQ(WordFormatter::flipu16(0x1234), 0x3412);
    CHECK_EQ(WordFormatter::flipu16(0x0000), 0x0000);
    CHECK_EQ(WordFormatter::flipu16(0xFFFF), 0xFFFF);
    CHECK_EQ(WordFormatter::flipu16(0xAA00), 0x00AA);
    CHECK_EQ(WordFormatter::flipu16(0xB380), 0x80B3);
}

TEST_CASE("32 bit words should be flipped correctly") {
    CHECK_EQ(WordFormatter::flipu32(0x12345678), 0x78563412);
    CHECK_EQ(WordFormatter::flipu32(0x00000000), 0x00000000);
    CHECK_EQ(WordFormatter::flipu32(0xFFFFFFFF), 0xFFFFFFFF);
    CHECK_EQ(WordFormatter::flipu32(0xAA00AA00), 0x00AA00AA);
    CHECK_EQ(WordFormatter::flipu32(0x5FD3963C), 0x3C96D35F);
}
