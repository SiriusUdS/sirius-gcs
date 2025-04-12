#include "WordFormatter.h"

uint16_t WordFormatter::flipu16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

uint32_t WordFormatter::flipu32(uint32_t val) {
    return (val << 24) | ((val & 0x0000FF00) << 8) | ((val & 0x00FF0000) >> 8) | (val >> 24);
}
